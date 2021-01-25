#pragma once
#include <stddef.h>
#include <stdint.h>


namespace Visionizer
{
    // This is existing as we do not want to have to make a memory-map for (example)32G. This is why we use bitmaps. 
    // A bitmap is up to 8x smaller than a memory map
    class Bitmap 
    {
    public:
        size_t Size;
        uint8_t* Buffer;
        bool operator[](uint64_t index); // Use like a normal array
        void Set(uint64_t index, bool value);
    };
}