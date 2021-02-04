#pragma once


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
#include "IO.h"
#include "interrupts/Interrupts.h"
#include "userinput/Mouse.h"

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

    
    struct KernelInfo
    {
        PageTableManager* pageTableManager;
    };
    KernelInfo InitializeKernel(KernelBootInfo* BootInfo);
}
