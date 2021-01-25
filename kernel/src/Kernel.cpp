#include <string>
#include <stdlib.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "Framebuffer.h"
#include "EfiMemory.h"
#include "Memory.h"
#include "Bitmap.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/Paging.h"
#include "paging/PageTableManager.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
namespace Visionizer
{
	struct KernelBootInfo
	{
		Framebuffer* framebuffer;
		PSF1_FONT* psf1_Font;
		EFI_MEMORY_DESCRIPTOR* mMap;
		uint64_t mMapSize;
		uint64_t mMapDescSize;
	};

	// Uses the renderer to generate a newline
	void NewLine(BasicRenderer* ren)
	{
		ren->CursorPosition = {0, ren->CursorPosition.Y + 16};
		return;
	}






	// The main
	extern "C" void _startKernel(KernelBootInfo* bootInfo)
	{
		BasicRenderer infoRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);


		uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

		GlobalAllocator = PageFrameAllocator();
		GlobalAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);


		// ALLOCATING FOR THE KERNEL

		// How much memory will our kernel need?
		uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;// Gives us size of kernel in mem

		// How many pages do we have to allocate for the Kernel?
		uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

		GlobalAllocator.LockPages(&_KernelStart, kernelPages); // It should be locked away by EFI, but just to make sure, we lock it again
		// ---
		// ----------------------------------------- COPYRIGHT NOTICE ----------------------------
		
		// We have to print a copyright notice
		infoRenderer.Print("Cletyxo Terminal");
		NewLine(&infoRenderer);
		infoRenderer.Print("Copyright (C) Visionizer Corporation. All rights reserved.");

		// -----------------------

		// Our PML4 (Highest Level) Address
		PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
		Memset(PML4, 0, 0x1000);

		PageTableManager pageTableManager = PageTableManager(PML4);

		for (uint64_t i = 0; i < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); i+= 0x1000){
			pageTableManager.MapMemory((void*)i, (void*)i);
		}

		uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
		uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
		for (uint64_t i = fbBase; i < fbBase + fbSize; i += 4096){
			pageTableManager.MapMemory((void*)i, (void*)i);
		}

		asm ("mov %0, %%cr3" : : "r" (PML4));

		pageTableManager.MapMemory((void*)0x600000000, (void*)0x80000);

		uint64_t* test = (uint64_t*)0x600000000;
		*test = 26;

		infoRenderer.Print(ToString(*test));
		
		return;
	}

}
