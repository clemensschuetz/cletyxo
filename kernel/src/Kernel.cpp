#include "KernelUtil.h"
#include "Panic.h"
#include "userinput/Mouse.h"

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
		GlobalRenderer->Print("Visionizer's SysMNG");
		GlobalRenderer->NextLine();
		GlobalRenderer->Print("Copyright (C) Clemens Schuetz, Visionizer & Cletyxo Core Contributors. No guarantees granted.");
		// -----------------------
		GlobalRenderer->NextLine();
		GlobalRenderer->NextLine();
		GlobalRenderer->NextLine();
		
		GlobalRenderer->Print("Kernel initialized successfully!");
		
		GlobalRenderer->NextLine();
		GlobalRenderer->Print(ToHString((uint64_t)bootInfo->rsdp));


		// The actual while(true) loop
		while(true)
		{
			ProcessMousePacket();
		}


		// Just here so UEFI wont shut our kernel down for not responding
		while(true);
	}

}
