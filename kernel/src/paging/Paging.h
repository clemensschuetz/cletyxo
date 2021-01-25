#pragma once
#include <stdint.h>

namespace Visionizer
{
    struct PageDirectoryEntry
    {
        bool Present : 1; // The thing is there
        bool ReadWrite : 1; // Is the page read/write
        bool UserSuper : 1; // Can the memory be accessed by both super and user or only super
        bool WriteThrough : 1;
        bool CacheDisabled : 1;
        bool Accessed : 1;
        bool ignore0 : 1;
        bool LargerPages : 1; // Is it larger than 512?
        bool ignore1 : 1;
        uint8_t Available : 3;
        uint64_t Address : 52;
        // All of this should equal 64 bits so that we can store it in a 64bit int
    };

    struct PageTable
    {
        PageDirectoryEntry entries[512]; // 512 pages max

    }__attribute__((aligned(0x10000)));

}