  #include "Bitmap.h"
  

namespace Visionizer
{
    bool Bitmap::operator[](uint64_t index){
        if (index > Size * 8) return false; // is it within its size (prevent buffer overflow)

        uint64_t byteIndex = index / 8;
        uint8_t bitIndex = index % 8;
        uint8_t bitIndexer = 0b10000000 >> bitIndex;
        if ((Buffer[byteIndex] & bitIndexer) > 0){
            return true;
        }
        return false;
    }

    bool Bitmap::Set(uint64_t index, bool value){
        if (index > Size * 8) return false; // Prevent buffer overflow

        uint64_t byteIndex = index / 8;
        uint8_t bitIndex = index % 8;
        uint8_t bitIndexer = 0b10000000 >> bitIndex;
        Buffer[byteIndex] &= ~bitIndexer;
        if (value){
            Buffer[byteIndex] |= bitIndexer;
        }

        return true;
    }
}
