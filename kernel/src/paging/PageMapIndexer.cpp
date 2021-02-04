

#include "PageMapIndexer.h"

namespace Visionizer
{
    PageMapIndexer::PageMapIndexer(uint64_t virtualAddress)
    {
        // Bitshifting
        virtualAddress >>= 12; // They are aligned by 12 bits.

        P_i = virtualAddress & 0x1ff;
        virtualAddress >>= 9;
        
        PT_i = virtualAddress & 0x1ff;
        virtualAddress >>= 9;

        PD_i = virtualAddress & 0x1ff;
        virtualAddress >>= 9;

        PDP_i = virtualAddress & 0x1ff;
    }
}