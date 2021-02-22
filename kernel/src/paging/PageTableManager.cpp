#include "PageTableManager.h"
#include "PageMapIndexer.h"
#include <stdint.h>
#include "PageFrameAllocator.h"
#include "../Memory.h"

namespace Visionizer
{

    PageTableManager GlobalPTM = NULL;

    PageTableManager::PageTableManager(PageTable* PML4Address)
    {
        // This may seem useless, but we never want the dev to forget to add a PML4, this is why we require it with a constructor.
        this->PML4 = PML4Address;
    }


    void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory)
    {
        // FIRST PAGE

        PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
        PageDirectoryEntry PDE;

        PDE = PML4->entries[indexer.PDP_i];
        PageTable* PDP;
        if (!PDE.GetFlag(PT_Flag::Present))
        {
            // It does not exist (yet)
            PDP = (PageTable*)GlobalAllocator.RequestPage();
            Memset(PDP, 0, 0x1000 /* One page */);
            PDE.SetAddress((uint64_t)PDP >> 12);
            PDE.SetFlag(PT_Flag::Present, true); // It is now true
            PDE.SetFlag(PT_Flag::ReadWrite, true); // We do not want readonly memory or? DECIDE
            PML4->entries[indexer.PDP_i] = PDE;
        }
        else
        {
            PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }

        // SECOND PAGE
        
        PDE = PDP->entries[indexer.PD_i];
        PageTable* PD;
        if (!PDE.GetFlag(PT_Flag::Present)){
            PD = (PageTable*)GlobalAllocator.RequestPage();
            Memset(PD, 0, 0x1000);
            PDE.SetAddress((uint64_t)PD >> 12);
            PDE.SetFlag(PT_Flag::Present, true);
            PDE.SetFlag(PT_Flag::ReadWrite, true);
            PDP->entries[indexer.PD_i] = PDE;
        }
        else
        {
            PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }

        // THIRD PAGE

        PDE = PD->entries[indexer.PT_i];
        PageTable* PT;
        if (!PDE.GetFlag(PT_Flag::Present)){
            PT = (PageTable*)GlobalAllocator.RequestPage();
            Memset(PT, 0, 0x1000);
            PDE.SetAddress((uint64_t)PT >> 12);
            PDE.SetFlag(PT_Flag::Present, true);
            PDE.SetFlag(PT_Flag::ReadWrite, true);
            PD->entries[indexer.PT_i] = PDE;
        }
        else
        {
            PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }

        PDE = PT->entries[indexer.P_i];
        PDE.SetAddress((uint64_t)physicalMemory >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PT->entries[indexer.P_i] = PDE;
    }
}