#pragma once


#include <stdint.h>
#include "EfiMemory.h"

namespace Visionizer
{
    uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);    
}
