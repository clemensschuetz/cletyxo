#pragma once
#include "Paging.h"

namespace Visionizer
{
    // We will have more than one at a time
    class PageTableManager 
    {
    public:
        // This constructor exists as you never want to forget the PML4 address :)
        PageTableManager(PageTable* PML4Address /* The highest level possible */); 

        // The PML4Address : 4 is the highest level
        PageTable* PML4;

        void MapMemory(void* virtualMemory, void* physicalMemory);
    };
    extern PageTableManager GlobalPTM;
}