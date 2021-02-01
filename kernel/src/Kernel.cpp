#include "KernelUtil.h"

namespace Visionizer
{
	// The main
	extern "C" void StartKernel(KernelBootInfo* bootInfo)
	{

		KernelInfo kernelInfo = InitializeKernel(bootInfo);

		PageTableManager* pageTableManager = kernelInfo.pageTableManager;
		// Actual start

		// ----------------------------------------- COPYRIGHT NOTICE ----------------------------
		
		// We have to print a copyright notice
//		infoRenderer.Print("Cletyxo Terminal");
//
//		infoRenderer.Print("Copyright (C) Visionizer Corporation. All d reserved.");
//		// -----------------------
//
		GlobalRenderer->Print("Kernel initialized successfully!");

		int* test = (int*)0x80000000000;
		*test = 2;

		while(true); // FIXME, I guess while true isnt always good
	}

}
