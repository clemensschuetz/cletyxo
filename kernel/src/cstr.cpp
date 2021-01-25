#include "cstr.h"



// DOCUMENTATION FOR THIS UTIL CLASS:
// Turns any value into a string
// BUT ATTENTION
// You have to cast the value to the type (uint64_t or int64_t) or you will get an overload error.

namespace Visionizer 
{
    char uintToStrintResult[128];
    const char* ToString(uint64_t value)
    {
        uint8_t size;
        uint64_t sizeTest = value;
        while (sizeTest / 10 > 0)
        {
            sizeTest /= 10;
            size++;
        }

        uint8_t index = 0;
        while (value / 10 > 0)
        {
            uint8_t remainder = value % 10;
            value /= 10;
            uintToStrintResult[size - index] = remainder + '0';
            index++;
        }

        uint8_t remainder = value % 10;
        uintToStrintResult[size - index] = remainder + '0';
        uintToStrintResult[size + 1] = 0;
        return uintToStrintResult;
    }


    // if its negative
    char intToStrintResult[128];
    const char* ToString(int64_t value)
    {
        uint8_t isNegative = 0; // If is negative = 1;
        if (value < 0)
        {
            isNegative = 1;
            value *= -1; // Turn it positive
            intToStrintResult[0] = '-'; // Its just a string, no calcs are done with it, so we just add a little '-' before it :)
        }

        uint8_t size;
        uint64_t sizeTest = value;
        while (sizeTest / 10 > 0)
        {
            sizeTest /= 10;
            size++;
        }

        uint8_t index = 0;
        while (value / 10 > 0)
        {
            uint8_t remainder = value % 10;
            value /= 10;
            intToStrintResult[isNegative + size - index] = remainder + '0'; // If it is negative, 0 is already claimed.
            index++;
        }

        uint8_t remainder = value % 10;
        intToStrintResult[isNegative + size - index] = remainder + '0';
        intToStrintResult[isNegative + size + 1] = 0;
        return intToStrintResult;
    }


    // TODO REMOVE DECIMALPLACES
    // if its a double (hexadecimal)
    char doubleToStringResult[128];
    const char* ToString(double value, uint8_t decimalPlaces)
    {
        if (decimalPlaces > 20) decimalPlaces = 20;


        char* intPtr = (char*)ToString((int64_t)value); // Chop off the decimals
        char* doublePtr = doubleToStringResult;


        if (value < 0)
        {
            value *= -1;
        }

        // Now we get all of the non-decimals
        while(*intPtr != 0)
        {
            *doublePtr = *intPtr;
            intPtr++;
            doublePtr++;
        }

        *doublePtr = '.';
        doublePtr++;

        double newValue = value - (int)value; // Value is full, other is only full: E.g.: Value is 10.4: double newValue = 10.4 - 10 (=4)

        for(uint8_t i = 0; i < decimalPlaces; i++)
        {
            newValue *= 10;
            *doublePtr = (int)newValue + '0';
            newValue -= (int)newValue;
            doublePtr++;
        }

        *doublePtr = 0; // null terminate it
        return doubleToStringResult;

    }

    const char* ToString(double value)
    {
        // Calculating the decimals
        // TODO Round to 2 decimals
        return ToString(value, 2);
    }

    char hexToStringResult[128];
    const char* ToHString(uint64_t value)
    {  
        uint64_t* valPtr = &value;
        uint8_t* ptr;
        uint8_t tmp;
        uint8_t size = 8 * 2 - 1;
        for (uint8_t i = 0; i < size; i++)
        {
            ptr = ((uint8_t*) valPtr + i);
            tmp = ((*ptr & 0xF0) >> 4);
            hexToStringResult[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');

            tmp = ((*ptr & 0x0F));
            hexToStringResult[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
        }
        hexToStringResult[size + 1] = 0;
        return hexToStringResult;
    }

    char hex32ToStringResult[128];
    const char* ToHString(uint32_t value)
    {  
        uint32_t* valPtr = &value;
        uint8_t* ptr;
        uint8_t tmp;
        uint8_t size = 4 * 2 - 1;
        for (uint8_t i = 0; i < size; i++)
        {
            ptr = ((uint8_t*) valPtr + i);
            tmp = ((*ptr & 0xF0) >> 4);
            hex32ToStringResult[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');

            tmp = ((*ptr & 0x0F));
            hex32ToStringResult[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
        }
        hex32ToStringResult[size + 1] = 0;
        return hex32ToStringResult;
    }

    char hex16ToStringResult[128];
    const char* ToHString(uint16_t value)
    {  
        uint16_t* valPtr = &value;
        uint8_t* ptr;
        uint8_t tmp;
        uint8_t size = 2 * 2 - 1;
        for (uint8_t i = 0; i < size; i++)
        {
            ptr = ((uint8_t*) valPtr + i);
            tmp = ((*ptr & 0xF0) >> 4);
            hex16ToStringResult[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');

            tmp = ((*ptr & 0x0F));
            hex16ToStringResult[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
        }
        hex16ToStringResult[size + 1] = 0;
        return hex16ToStringResult;
    }

    char hex8ToStringResult[128];
    const char* ToHString(uint8_t value)
    {  
        uint8_t* valPtr = &value;
        uint8_t* ptr;
        uint8_t tmp;
        uint8_t size = 1 * 2 - 1;
        for (uint8_t i = 0; i < size; i++)
        {
            ptr = ((uint8_t*) valPtr + i);
            tmp = ((*ptr & 0xF0) >> 4);
            hex8ToStringResult[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');

            tmp = ((*ptr & 0x0F));
            hex8ToStringResult[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
        }
        hex8ToStringResult[size + 1] = 0;
        return hex8ToStringResult;
    }
}