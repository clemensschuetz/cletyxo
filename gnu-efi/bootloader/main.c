#include <efi.h>
#include <efilib.h>
#include <elf.h>



typedef unsigned long long size_t;

// Font loading
#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

// The struct of a Framebuffer :)
typedef struct 
{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanline;
} Framebuffer;

// The next two are for font loading :=)
typedef struct 
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct 
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;

} PSF1_FONT;


// Initialize our GOP (Responsible for Graphic Drawing, etc)
Framebuffer framebuffer;
Framebuffer* InitializeGOP() // TODO REMOVE: BAD FOR PERFORMANCE
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
	if (EFI_ERROR(status)) 
	{
		Print(L"[Bootloader] Unable to locate the Graphics Output Protocol (GOP). Please restart your computer \n\r");
		return NULL;
	}
	else 
	{
		Print(L"[Bootloader] Graphics Output Protocol (GOP) successfully located. \n\r");
	}
	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanline = gop->Mode->Info->PixelsPerScanLine; // TODO Add like 1 or 2 to it in order for some things to work bette

	return &framebuffer;
}


// Load a file (primitive, just loads file, does not read it)
EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) 
{
	EFI_FILE* LoadedFile;
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if (Directory == NULL)
	{
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS)
	{
		return NULL;
	} 
	return LoadedFile;

}

// Load a psf version 1 font into memory
PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (font == NULL) return NULL;

	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontHeader);
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontHeader);

	if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1)
	{
		return NULL;
	}

	UINTN glyphBufferSize = fontHeader->charsize * 256;
	if (fontHeader->mode == 1) 
	{ // 512 glyph mode
		glyphBufferSize = fontHeader->charsize * 512;
	}

	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;
}

// Compare 2 memory addreses TODO make more efficient
int memcmp(const void* aptr, const void* bptr, size_t n)
{
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++)
	{
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

// Our Kernel struct (With everything that our kernel needs)
typedef struct 
{
	Framebuffer* s_Framebuffer;
	PSF1_FONT* s_PSF1_Font;
	EFI_MEMORY_DESCRIPTOR* s_Map;
	UINTN s_MapSize;
	UINTN s_MapDescriptorSize;

} KernelBootInfo;


// The entire main triggered on boot
EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);

	Print(L"Visionizer Cletyxo [ Version 0.1 Pre-Alpha ] \n\r"); // Get version automatically TODO
	Print(L"(c) 2021 Visionizer Corporation. All rights reserverd \n\r");
	Print(L"\n\r");
	Print(L"Initializing Startup Sequence... \n\r");
	Print(L"\n\r");
	Print(L"[Bootloader] He lā maikaʻi, my name is Alex and I am your bootloader. I will take care of the booting and setup your entire system. Hoʻomaha, no need to worry, I will tell you exactly whats wrong. :) \n\r");
	Print(L"\n\r");

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if(Kernel == NULL) 
	{
		Print(L"[Bootloader] Fatal: Could not verify existance of kernel.elf. Please reinstall the system or reboot your computer.\n\r"); // Better msg TODO
	} // Maybe add checkmarks TODO
	else {
		Print(L"[Bootloader] Success: Kernels existance verified. \n\r");
	}


	// Verify the Kernel format, etc
	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	} if(
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		Print(L"[Bootloader] Kernel format is not correct. Please restart or reinstall the software \n\r");
	}
	else {
		Print(L"[Bootloader] Kernel format successfully verified. \n\r");
	}



	// Load Kernel
	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for(
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	)
	{
		switch (phdr->p_type)
		{
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void**)segment);
				break;
			}
		}
	}

	Print(L"[Bootloader] Kernel was successfully loaded. \n\r");



	// Loading the font...
	PSF1_FONT* newFont = LoadPSF1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable);
	// Is it null?
	Print(L"\n\r");
	if (newFont == NULL)
	{ // TODO Maybe add fallback font
		Print(L"[Bootloader] Font %s could not be loaded: Returning to fallback font", "zap-light16.psf \n\r"); // TODO MAke automatic // FIXME not working with %s
		Print(L"[Bootloader] Failed to load Fallback Font: Returning to system standard font \n\r"); // TODO obvious
	}
	else 
	{
		Print(L"[Bootloader] Font %s was successfully loaded! Character size: %d \n\r", "zap-light16", newFont->psf1_Header->charsize); // TODO Make this automatic too
	}

	// Initializing the graphics
	Framebuffer* newBuffer = InitializeGOP();

	// Printing Information
	Print(
		L"\n\r"
		L"[Bootloader, Graphics Output Protocol (GOP)] Your display information: \n\r"
		"[GOP] Base: 0x%x \n\r" // Without the 0x people wont realize that this is a byte which will lead them to think that it is something weird
		"[GOP] Size: 0x%x \n\r" // Same
		"[GOP] Width: %d \n\r"
		"[GOP] Height: %d \n\r"
		"[GOP] Pixels Per Scan Line: %d \n\r",
		newBuffer->BaseAddress,
		newBuffer->BufferSize, 
		newBuffer->Width, 
		newBuffer->Height,
		newBuffer->PixelsPerScanline);



	// Memory Maps
	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	{
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

	Print(L"\n\r");
	Print(L"[Bootloader] I am Alex, remember me? Your bootloader. Anyways, goodbye. My job is done \n\r");





	// Loading the kernel
	void (*KernelStart)(KernelBootInfo*) = ((__attribute__((sysv_abi)) void (*)(KernelBootInfo*) ) header.e_entry);

	// Setting up our kernel struct
	KernelBootInfo kernelBootInfo;
	kernelBootInfo.s_Framebuffer = newBuffer;
	kernelBootInfo.s_PSF1_Font = newFont;
	kernelBootInfo.s_Map = Map;
	kernelBootInfo.s_MapSize = MapSize;
	kernelBootInfo.s_MapDescriptorSize = DescriptorSize;




	// Exiting the boot services..
	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

	KernelStart(&kernelBootInfo);

	return EFI_SUCCESS; // Exit the UEFI application
}
