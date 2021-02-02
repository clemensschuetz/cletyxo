#include "KernelUtil.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "interrupts/Interrupts.h"

namespace Visionizer
{
    KernelInfo kernelInfo;
    PageTableManager pageTableManager = NULL;

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

        pageTableManager = PageTableManager(PML4);

        for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t+= 0x1000){
            pageTableManager.MapMemory((void*)t, (void*)t);
        }

        uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
        uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
        GlobalAllocator.LockPages((void*)fbBase, fbSize/ 0x1000 + 1);
        for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
            pageTableManager.MapMemory((void*)t, (void*)t);
        }

        asm ("mov %0, %%cr3" : : "r" (PML4));

        kernelInfo.pageTableManager = &pageTableManager;
    }

    // Our IDT Descriptor Table
    IDTR idtr; 
    void PrepareInterrupts()
    {
        idtr.Limit = 0x0FFF; // Size of a full idtr
        idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();


        // Adding the PageFaultEntry
        IDTDescEntry* int_PageFault = (IDTDescEntry*)(idtr.Offset + 0xE * sizeof(IDTDescEntry));
        int_PageFault->SetOffset((uint64_t)PageFault_Handler);
        int_PageFault->type_attr = IDT_TA_InterruptGate;
        int_PageFault->selector = 0x08
        
        // Adding the DoubleFaultEntry
        IDTDescEntry* int_DoubleFault = (IDTDescEntry*)(idtr.Offset + 0x8 * sizeof(IDTDescEntry));
        int_DoubleFault->SetOffset((uint64_t)DoubleFault_Handler);
        int_DoubleFault->type_attr = IDT_TA_InterruptGate;
        int_DoubleFault->selector = 0x08;
        
        // Adding the GPFaultEntry
        IDTDescEntry* int_GPFault = (IDTDescEntry*)(idtr.Offset + 0xD * sizeof(IDTDescEntry));
        int_GPFault->SetOffset((uint64_t)GPFault_Handler);
        int_GPFault->type_attr = IDT_TA_InterruptGate;
        int_GPFault->selector = 0x08;


        asm ("lidt %0" : : "m" (idtr));
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

        return kernelInfo;
    }
}