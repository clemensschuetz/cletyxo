#include "KernelUtil.h"

namespace Visionizer
{
	// The main
	extern "C" void _startKernel(KernelBootInfo* bootInfo)
	{

		KernelInfo kernelInfo = InitializeKernel(bootInfo);

		PageTableManager* pageTableManager = kernelInfo.pageTableManager;
		// Actual start
		BasicRenderer infoRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);


		// ----------------------------------------- COPYRIGHT NOTICE ----------------------------
		
		// We have to print a copyright notice
//		infoRenderer.Print("Cletyxo Terminal");
//
//		infoRenderer.Print("Copyright (C) Visionizer Corporation. All d reserved.");
//		// -----------------------
//
//		infoRenderer.Print("Kernel successfully initialized. Have fun!");



		while(true); // FIXME, I guess while true isnt always good
	}

}
