#ifndef _flash_bitmap_h
#define _flash_bitmap_h

/**
 * @file	flash_bitmap.h
 * @author	en2
 * @date	28-04-2020
 * @brief	
 * @details	
**/

#include "string.h"

template<int size>
class Flash_bitmap
{   
    static_assert(size % 8 == 0, "Invalid size, must be byte aligned ...");
    static constexpr unsigned char _mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

public:
    bool get(int bit);
    Flash_bitmap & set(int bit, bool value);
    
    Flash_bitmap & fill(bool value);

    int count(bool key);
    bool is_full(bool key);

private:
    unsigned char _byte[size / 8];

}; /* class: Flash_bitmap */

template<int size>
bool Flash_bitmap<size>::get(int bit)
{
    return (_byte[bit / 8] & _mask[bit % 8]);
}

template<int size>
Flash_bitmap<size> & Flash_bitmap<size>::set(int bit, bool value)
{
    value ? ( _byte[bit / 8] |= _mask[bit % 8]) : (_byte[bit / 8] &= ~_mask[bit % 8]);

    return *this;
}

template<int size>
Flash_bitmap<size> & Flash_bitmap<size>::fill(bool value)
{
    value ? memset(_byte, 0xff, size / 8) : memset(_byte, 0, size / 8);

    return *this;
}

template<int size>
int Flash_bitmap<size>::count(bool key)
{
    int count = 0;

    for (int i = 0; i < size; i++)
    {
        if (get(i) == key) count++;
    }

    return count;
}

template<int size>
bool Flash_bitmap<size>::is_full(bool key)
{
    return (count(key) == size);
}

#endif /* define: flash_bitmap_h */