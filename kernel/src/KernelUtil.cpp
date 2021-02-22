#include "KernelUtil.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "userinput/Mouse.h"


namespace Visionizer
{
    KernelInfo kernelInfo;

    

    // Prepares paging, etc.
    void PrepareMemory(KernelBootInfo* bootInfo)
    {

        uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

        GlobalAllocator = PageFrameAllocator();
        GlobalAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

        uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
        uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

        GlobalAllocator.LockPages(&_KernelStart, kernelPages);

        PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
        Memset(PML4, 0, 0x1000);

        GlobalPTM = PageTableManager(PML4);

        for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t+= 0x1000){
            GlobalPTM.MapMemory((void*)t, (void*)t);
        }

        uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
        uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
        GlobalAllocator.LockPages((void*)fbBase, fbSize/ 0x1000 + 1);
        for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
            GlobalPTM.MapMemory((void*)t, (void*)t);
        }

        asm ("mov %0, %%cr3" : : "r" (PML4));

        kernelInfo.pageTableManager = &GlobalPTM;
    }

    // Our IDT Descriptor Table
    IDTR idtr; 

    void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector)
    {
        IDTDescEntry* entry = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
        entry->SetOffset((uint64_t)handler);
        entry->type_attr = type_attr;
        entry->selector = selector;
    }




    void PrepareInterrupts()
    {

        // PREPARING INTERRUPTS

        idtr.Limit = 0x0FFF; // Size of a full idtr
        idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();


        // Adding the PageFaultEntry
        SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
        // Adding the DoubleFaultEntry
        SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
        // Adding the GPFaultEntry
        SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
        // Adding the Keyboard Entry
        SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);
        // Adding the Mouse Entry
        SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);

        asm ("lidt %0" : : "m" (idtr));

        // PREPARING PIC-RELATED THINGS (KEYBOARD, MOUSE, PIC-CHIPS, etc)

        // Remapping the PIC
        RemapPIC();

        // Preparing the keyboard






    }

    void PrepareACPI(KernelBootInfo* bootInfo)
    {
        ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(bootInfo->rsdp->XSDTAddress);

        ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");

        PCI::EnumeratePCI(mcfg);
        
    }

    BasicRenderer r = BasicRenderer(NULL, NULL);
    // Initializes the kernel TODO, make it callable  max of once
    KernelInfo InitializeKernel(KernelBootInfo* bootInfo)
    {
        // Initialize Global Renderer   
        r = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);
        GlobalRenderer = &r;

        GDTDescriptor gdtDescriptor;
        gdtDescriptor.Size = sizeof(GDT) - 1;
        gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
        LoadGDT(&gdtDescriptor);
        

        PrepareMemory(bootInfo);

        // Reset the Entire Screen to black
        Memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);

        PrepareInterrupts();

        InitPS2Mouse(); // TODO More mouse support

        PrepareACPI(bootInfo);

        // Unmasking the keyboard
        outb(PIC1_DATA, 0b11111001);
        outb(PIC2_DATA, 0b11111111);


        // Enables the masking
        asm ("sti");

        Point pos;
        pos.X = 200;
        pos.Y = 200;
        GlobalRenderer->DrawOverlayMouseCursor(MousePointer, pos, 0x00ff0000);

        return kernelInfo;
    }
}