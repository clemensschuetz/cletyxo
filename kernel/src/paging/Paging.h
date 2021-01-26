#pragma once
#include <stdint.h>

namespace Visionizer
{

    enum PT_Flag
    {
        Present = 0,
        ReadWrite = 1,
        UserSuper = 2,
        WriteThrough = 3,
        CacheDisabled = 4,
        Accessed = 5,
        LargetPages = 7,
        Custom0 = 9,
        Custom1 = 10,
        Custom2 = 11,
        NX = 63 // Only if supported
    };


    struct PageDirectoryEntry
    {
        uint64_t Value;
        // Sets any of the flags
        void SetFlag(PT_Flag flag, bool enabled);
        // Gets if the selected flag is true or false
        bool GetFlag(PT_Flag flag);
        // Sets the address
        void SetAddress(uint64_t address);
        // Returns the address
        uint64_t GetAddress();
    };

    struct PageTable
    {
        PageDirectoryEntry entries[512]; // 512 pages max

    }__attribute__((aligned(0x10000)));

}