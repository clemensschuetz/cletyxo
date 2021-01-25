#pragma once
#include "EfiMemory.h"
#include <stdint.h>
#include "Bitmap.h"
#include "Memory.h"

namespace Visionizer
{
    class PageFrameAllocator
    {
    public:
        void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize);
        Bitmap PageBitmap;

        // Free a memory page
        void FreePage(void* address);

        // Locks a page (uses it)
        void LockPage(void* address);

        // Free multiple pages - Starts of address and continues to lock the pages after it (indicated by pageCount)
        void FreePages(void* address, uint64_t pageCount);

        // Locks multiple pages - Starts of address and continues to lock the pages after it (indicated by pageCount)
        void LockPages(void* address, uint64_t pageCount);

        
        // -------- GETTERS -----------
        void* RequestPage();
        
        uint64_t GetFreeRAM();
        uint64_t GetUsedRAM();
        uint64_t GetReservedRAM();

    private:
        void InitBitmap(size_t bitmapSize, void* bufferAdress);

        // Reserves a page
        void ReservePage(void* address);

        // Unreserve a reserved page
        void UnreservePage(void* address);

        // Reserves multiple pages - Starts of address and continues to reserve the pages after it (indicated by pageCount)
        void ReservePages(void* address, uint64_t pageCount);

        // Unreserve multiple reserved pages - Starts of address and continues to unreserve the pages after it (indicated by pageCount)
        void UnreservePages(void* address, uint64_t pageCount);
    };
}