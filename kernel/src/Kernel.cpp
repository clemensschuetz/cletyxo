#include <string>
#include <stdlib.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "Framebuffer.h"
#include "EfiMemory.h"
#include "Memory.h"
#include "Bitmap.h"
#include "PageFrameAllocator.h"


struct KernelBootInfo
{
	Visionizer::Framebuffer* framebuffer;
	Visionizer::PSF1_FONT* psf1_Font;
	Visionizer::EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

// Uses the renderer to generate a newline
void NewLine(Visionizer::BasicRenderer* ren)
{
	ren->CursorPosition = {0, ren->CursorPosition.Y + 16};
	return;
}

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;






// The main
extern "C" void _startKernel(KernelBootInfo* bootInfo)
{
	Visionizer::BasicRenderer infoRenderer = Visionizer::BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);

	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

	Visionizer::PageFrameAllocator mainAllocator;
	mainAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);


	// ALLOCATING FOR THE KERNEL

	// How much memory will our kernel need?
	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;// Gives us size of kernel in mem

	// How many pages do we have to allocate for the Kernel?
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	mainAllocator.LockPages(&_KernelStart, kernelPages); // It should be locked away by EFI, but just to make sure, we lock it again
	// ---


	NewLine(&infoRenderer);
	infoRenderer.Print("Free RAM: ");
	infoRenderer.Print(Visionizer::ToString(mainAllocator.GetFreeRAM() / 1024 /* Into kilobytes */ )); 
	infoRenderer.Print(" KB");

	NewLine(&infoRenderer);
	infoRenderer.Print("Used RAM: ");
	infoRenderer.Print(Visionizer::ToString(mainAllocator.GetUsedRAM() / 1024));
	infoRenderer.Print(" KB");

	NewLine(&infoRenderer);
	infoRenderer.Print("Reserved RAM: ");
	infoRenderer.Print(Visionizer::ToString(mainAllocator.GetReservedRAM() / 1024));
	infoRenderer.Print(" KB");

	for (int i = 0; i < 20; i++)
	{
		void* address = mainAllocator.RequestPage();
		NewLine(&infoRenderer);
		infoRenderer.Print(Visionizer::ToHString((uint64_t)address));
	}

	

	
    return;
}
