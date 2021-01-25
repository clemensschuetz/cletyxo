#include "PageFrameAllocator.h"


namespace Visionizer
{
    // Amount of free memory
    uint64_t freeMemory;
    // Memory that has been reserved (e.g. Bootlader)
    uint64_t reservedMemory;
    // The memory that we use for applications, etc
    uint64_t usedMemory;
    // Has it been initialized? Do not initialize more than once
    bool initialized = false;

    // Reads the Efi memory map
    void PageFrameAllocator::ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize)
    {
        if (initialized) return;
        initialized = true;

        uint64_t mMapEntries = mMapSize / mMapDescSize;

        void* largestFreeMemSegSoFar = NULL;
        size_t largestFreeMemSegSoFarSize = 0;

        for (int i = 0; i < mMapEntries; i++)
        {
            EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
            if (desc->type == 7) // 7 is the ID of the EFI Conventional Memory
            {
                // Which section is the largest?
                if (desc->numPages * 4096 /* bytes */ > largestFreeMemSegSoFarSize)
                {
                    largestFreeMemSegSoFar = desc->physAddr;
                    largestFreeMemSegSoFarSize = desc->numPages * 4096; // Bytes
                } 
            }
        }

        // Total memory Size of system
        uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescSize);
        freeMemory = memorySize; 
        uint64_t bitmapSize = memorySize / 4096 /* Bytes */ / 8 + 1;

        // Initialize Bitmap
        InitBitmap(bitmapSize, largestFreeMemSegSoFar);

        // Locking pages of our Bitmap because we do not want our bitmap to become corrupted :)

        LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);

        //  Reserve pages of unusable / reserved memory.
        for (int i = 0; i < mMapEntries; i++)
        {
            EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
            if (desc->type != 7) // Indicating that it is not EFI Conventional Memory (7)
            {
                ReservePages(desc->physAddr, desc->numPages);
            }
        }
    }



    // Initialize the Bitmap. Only call on system startup
    void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAdress)
    {
        PageBitmap.Size = bitmapSize;
        PageBitmap.Buffer = (uint8_t*)bufferAdress;
        for (int i = 0; i < bitmapSize; i++)
        {
            *(uint8_t*)(PageBitmap.Buffer + i) = 0;
        }
    }

    // ------------ SINGLE PAGE ACTIONS ---------------   

    // Free a memory page
    void PageFrameAllocator::FreePage(void* address)
    {
        uint64_t index = (uint64_t)address / 4096; // Get Bytes
        if (PageBitmap[index] == false) return; // If it is false, it is already free and why free a free page.
        PageBitmap.Set(index, false); // Now, we are making it free
        freeMemory += 4096; // The amount of one page
        usedMemory -= 4096; // Removing the amount of one page
    }
    // Lock a page from access
    void PageFrameAllocator::LockPage(void* address)
    {
        uint64_t index = (uint64_t)address / 4096; // Get Bytes
        if (PageBitmap[index] == true) return; // If it is true, it is already locked, and you shouldnt use an already used page
        PageBitmap.Set(index, true); // Now, we are locking it
        freeMemory -= 4096; // The amount of one page
        usedMemory += 4096; // Adding the amount of one page
    }
    // Reserves a page
    void PageFrameAllocator::ReservePage(void* address)
    {
        // Same comments as above (LockPage())
        uint64_t index = (uint64_t)address / 4096; 
        if (PageBitmap[index] == true) return; 
        PageBitmap.Set(index, true); 
        freeMemory -= 4096; 
        reservedMemory += 4096; 
    }
    // Unreserve a reserved page
    void PageFrameAllocator::UnreservePage(void* address)
    {
        // Same comments as in FreePage()
        uint64_t index = (uint64_t)address / 4096;
        if (PageBitmap[index] == true) return;
        PageBitmap.Set(index, true);
        freeMemory -= 4096;
        reservedMemory += 4096;
    }

    // ------------ MULTIPLE PAGES ACTIONS ---------------   

    void PageFrameAllocator::FreePages(void* address, uint64_t pageCount)
    {
        for (int i = 0; i < pageCount; i++)
        {
            FreePage((void*)((uint64_t)address + (i * 4096))); // Now we start off an dress and continue until we are finished
        }
    }

    void PageFrameAllocator::LockPages(void* address, uint64_t pageCount)
    {
        for (int i = 0; i < pageCount; i++)
        {
            LockPage((void*)((uint64_t)address + (i * 4096))); // Now we start off an dress and continue until we are finished
        }
    }


    void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount)
    {
        for (int i = 0; i < pageCount; i++)
        {
            UnreservePage((void*)((uint64_t)address + (i * 4096))); // Now we start off an dress and continue until we are finished
        }
    }

    void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount)
    {
        for (int i = 0; i < pageCount; i++)
        {
            ReservePage((void*)((uint64_t)address + (i * 4096))); // Now we start off an dress and continue until we are finished
        }
    }

    // TODO, FIXME this is horrible (not optimized!)
    // This function will return a not-used page
    void* PageFrameAllocator::RequestPage()
    {
        for (uint64_t i = 0; i < PageBitmap.Size * 8; i++)
        {
            if (PageBitmap[i] == true) continue; // If used, continue, otherwise, use it
            LockPage((void*)(i * 4096));
            return (void*)(i * 4096);
        }
        // Nothing found :( (Nothing left)
        // Now we have to swap (Saving memory to the disk in order to save memory)

        return NULL; // Page-frame Swap to file TODO FIXME

    }

    // ---------------------- GETTERS --------------------------
    uint64_t PageFrameAllocator::GetFreeRAM()
    {
        return freeMemory;
    }

    uint64_t PageFrameAllocator::GetUsedRAM()
    {
        return usedMemory;
    }

    uint64_t PageFrameAllocator::GetReservedRAM()
    {
        return reservedMemory;
    }

}