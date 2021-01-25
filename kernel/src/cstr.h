#pragma once
#include <stdint.h>

namespace Visionizer
{
	typedef char* VString;
	// IF YOU ARE HERE BECAUSE OF AN OVERLOAD ERROR, YOU NEED TO CAST THE NUMBER THAT YOU WANT TO
	// TURN INTO A STRING TO THE RIGHT TYPE
    const char* ToString(uint64_t value); // Only unsigned ints
    const char* ToString(int64_t value); // Only signed ints
    const char* ToString(double value, uint8_t decimalPlaces); // Only floats, with decimals
    const char* ToString(double value); // Only floats
    const char* ToHString(uint64_t value); // Makes HexString
    const char* ToHString(uint32_t value); // Makes HexString with 32bit
    const char* ToHString(uint16_t value); // Makes HexString with 16bit
    const char* ToHString(uint8_t value); // Makes HexString with 8bit

    // void appendToCharPtr(char* orig, size_t size, char c); // Appends to a thing https://stackoverflow.com/questions/12939370/c-appending-char-to-char
}
