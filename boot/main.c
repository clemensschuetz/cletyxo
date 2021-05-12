#include <efi.h>
#include <efilib.h>
#include <elf.h>

#define OK_FMT(fmt, ...) Print(L"[  OK  ] "); Print(L##fmt, __VA_ARGS__); Print(L"\n\r");
#define ERR_FMT(fmt, ...) Print(L"[  ERR ] "); Print(L##fmt, __VA_ARGS__); Print(L"\n\r");
#define INFO_FMT(fmt, ...) Print(L"[ INFO ] "); Print(L##fmt, __VA_ARGS__); Print(L"\n\r");

#define palloc(PoolType, Size, Buffer) Table->BootServices->AllocatePool(PoolType, Size, Buffer);
#define pagoc(Type, MemType, Pages, Seg) Table->BootServices->AllocatePages(Type, MemType, Pages, Seg);

#define OK(fmt) Print(L"[  OK  ] "); Print(L##fmt); Print(L"\n\r");
#define ERR(fmt) Print(L"[  ERR ] "); Print(L##fmt); Print(L"\n\r");
#define INFO(fmt) Print(L"[ INFO ] "); Print(L##fmt); Print(L"\n\r");

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef unsigned long long size_t;

struct Framebuffer {
    void* BaseAddr;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelsPerScanline;
};

struct PSF1_HDR {
    unsigned char Magic[2];
    unsigned char Mode;
    unsigned char Charsize;
};

struct PSF1_Font {
    struct PSF1_HDR* Hdr;
    void* GlyphBuffer;
};

struct Handover {
    struct Framebuffer* framebuffer;
    struct PSF1_Font* font;
};


struct Framebuffer framebuffer;
struct Framebuffer* InitializeGOP() {
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_STATUS status;

    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    if (EFI_ERROR(status)) {
        return NULL;
    }

    framebuffer.BaseAddr = (void*)gop->Mode->FrameBufferBase;
    framebuffer.BufferSize = gop->Mode->FrameBufferSize;
    framebuffer.Width = gop->Mode->Info->HorizontalResolution;
    framebuffer.Height = gop->Mode->Info->VerticalResolution;
    framebuffer.PixelsPerScanline = gop->Mode->Info->PixelsPerScanLine;

    return &framebuffer;
}


EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE Handle, EFI_SYSTEM_TABLE* Table)
{
    EFI_FILE* LoadedFile;

    EFI_LOADED_IMAGE_PROTOCOL* LoadedImg;
    Table->BootServices->HandleProtocol(Handle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImg);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FS;
    Table->BootServices->HandleProtocol(LoadedImg->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FS);

    if (Directory == NULL) {
        FS->OpenVolume(FS, &Directory);
    }

    EFI_STATUS status = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

    if (status != EFI_SUCCESS)
    {
        return NULL;
    }
    return LoadedFile;
}

struct PSF1_Font* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE Handle, EFI_SYSTEM_TABLE* Table) 
{
    EFI_FILE* font = LoadFile(Directory, Path, Handle, Table);
    if (font == NULL) return NULL;// File not found

    struct PSF1_HDR* fontHdr;
    palloc(EfiLoaderData, sizeof(struct PSF1_HDR), (void**)&fontHdr);
    UINTN size = sizeof(struct PSF1_HDR);
    font->Read(font, &size, fontHdr);

    UINTN glyphBufferSize = fontHdr->Charsize * 256;
    if (fontHdr->Mode == 1) { // 512 glyph mode
        glyphBufferSize = fontHdr->Charsize * 256;
    }

    void* glyphBuffer;
    {
        font->SetPosition(font, sizeof(struct PSF1_HDR));
        palloc(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
        font->Read(font, &glyphBufferSize, glyphBuffer);
    }

    struct PSF1_Font* finalFont;
    palloc(EfiLoaderData, sizeof(struct PSF1_Font), (void**)&finalFont);
    finalFont->Hdr = fontHdr;
    finalFont->GlyphBuffer = glyphBuffer;

    if (fontHdr->Magic[0] == PSF1_MAGIC0 && fontHdr->Magic[1] == PSF1_MAGIC1) {
        return finalFont;
    } else {
        return NULL; // Bad format
    }
}


int memcmp(const void* aptr, const void* bptr, size_t n) {
    const unsigned char* a = aptr, *b = bptr;
    for (size_t i = 0; i < n; i++) {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }
    return 0;
}

EFI_STATUS efi_main(EFI_HANDLE Handle, EFI_SYSTEM_TABLE* Table) {
    InitializeLib(Handle, Table);
    INFO("Starting bootloader...");

    // Checking if the kernel even exists
    // If it doesn't, we obv don't want to continue :)
    EFI_FILE* Cletyxo = LoadFile(NULL, L"cletyxo.elf", Handle, Table);
    if (Cletyxo == NULL)
    {
        ERR("Could not load kernel. Please check your install!");
        return EFI_LOAD_ERROR;
    } else {
        OK("Kernel loaded successfully!");
    }

    Elf64_Ehdr hdr;
    {
        UINTN FileInfoSize;
        EFI_FILE_INFO* FileInfo;
        Cletyxo->GetInfo(Cletyxo, &gEfiFileInfoGuid, &FileInfoSize, NULL);
        palloc(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
        Cletyxo->GetInfo(Cletyxo, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);
        UINTN size = sizeof(hdr);
        Cletyxo->Read(Cletyxo, &size, &hdr);
    }

    if (
        memcmp(&hdr.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
        hdr.e_ident[EI_CLASS] != ELFCLASS64                 ||
        hdr.e_ident[EI_DATA] != ELFDATA2LSB                 ||
        hdr.e_type != ET_EXEC                               ||
        hdr.e_machine != EM_X86_64                          ||
        hdr.e_version != EV_CURRENT
    )
    {
        ERR("The format of the kernel is bad. Please check your installation!");
        return EFI_UNSUPPORTED;
    }
    else
    {
        OK("Kernel header successfully verified.");
    }

    // Getting all of the program headers
    Elf64_Phdr* phdrs;
    {
        Cletyxo->SetPosition(Cletyxo, hdr.e_phoff);
        UINTN size = hdr.e_phnum * hdr.e_phentsize;
        palloc(EfiLoaderData, size, (void**)&phdrs);
        Cletyxo->Read(Cletyxo, &size, phdrs);
    }

    // Iterating over each program header
    for (
        Elf64_Phdr* phdr = phdrs;
        (char*)phdr < (char*)phdrs + hdr.e_phnum * hdr.e_phentsize;
        phdr = (Elf64_Phdr*)((char*)phdr + hdr.e_phentsize)
    )
    {
        // Doing things based on it's type
        switch (phdr->p_type)
        {
            case PT_LOAD:
                int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
                Elf64_Addr segment = phdr->p_paddr;
                pagoc(AllocateAddress, EfiLoaderData, pages, &segment);
                Cletyxo->SetPosition(Cletyxo, phdr->p_offset);
                UINTN size = phdr->p_filesz;
                Cletyxo->Read(Cletyxo, &size, (void*)segment);
                break;
        }
    }

    OK("Kernel successfully loaded!");
    INFO("Continuing to kernel starting phase...");

    // Kernel function pointer
    void (*KernelStart)(struct Handover*) = ((__attribute__((sysv_abi)) void (*)(struct Handover*) ) hdr.e_entry);

    struct Framebuffer* buffer = InitializeGOP();
    if (buffer == NULL) {
        ERR("Failed to locate Graphics Output Protocol (GOP)");
        return EFI_PROTOCOL_ERROR;
    } else {
        OK("Located Graphics Output Protocol (GOP)");
    }
    Print(L"\n\r");
    INFO("Printing framebuffer information:");
    INFO_FMT("  Base:              0x%x", buffer->BaseAddr);
    INFO_FMT("  Size:              0x%x", buffer->BufferSize);
    INFO_FMT("  Width:             %d", buffer->Width);
    INFO_FMT("  Height:            %d", buffer->Height);
    INFO_FMT("  PixelsPerScanline: %d", buffer->PixelsPerScanline);
    Print(L"\n\r");

    // Loading the font
    struct PSF1_Font* font = LoadPSF1Font(NULL, L"defaultfont.psf", Handle, Table);
    if (font == NULL) {
        ERR("Font is not valid / not found. This is most likely a problem with your installation. Please reinstall.");
    } else {
        Print(L"\n\r");
        OK("Font was found. General Information:");
        INFO_FMT("CharSize: %d", font->Hdr->Charsize);
        Print(L"\n\r");
    }

    // Creating the handover
    struct Handover over;
    over.framebuffer = buffer;
    over.font = font;

    KernelStart(&over);
    OK("Started kernel!");

    Print(L"\n\r");
    Print(L"\n\r");

    INFO("If you see this message that means that something went wrong!");
    INFO("If the Operating System (OS) worked, then you wouldn't be here!");
    INFO("The first action that it will do is the clearing of this screen.");
    INFO("Please create an issue on GitHub.");

    return EFI_SUCCESS;
}