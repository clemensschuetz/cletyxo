#include "KernelUtil.h"
#include "gdt/gdt.h"

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



    // Initializes the kernel TODO, make it callable  max of once
    KernelInfo InitializeKernel(KernelBootInfo* bootInfo)
    {
        GDTDescriptor gdtDescriptor;
        gdtDescriptor.Size = sizeof(GDT) - 1;
        gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
        LoadGDT(&gdtDescriptor);

        PrepareMemory(bootInfo);

        Memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);

        return kernelInfo;
    }
}