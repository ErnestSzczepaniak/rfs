#ifndef _bitmap_h
#define _bitmap_h

/**
 * @file	bitmap.h
 * @author	en2
 * @date	22-04-2020
 * @brief	
 * @details	
**/

#include "string.h"

template<int size>
class Bitmap
{   
    static_assert(size % 8 == 0);

public:
    bool get(int bit);
    Bitmap & set(int bit, bool value);

    int count(bool value);

    bool is_full_of(bool value);

    Bitmap & reset(bool value);
    int allocate(bool value);

private:
    unsigned char _byte[size / 8];

}; /* class: Flash_bitmap */

template<int size>
bool Bitmap<size>::get(int bit)
{
    if (bit < size)
    {
        return _byte[bit / 8] & (0x80 >> (bit % 8));
    }
    else return false;
}

template<int size>
Bitmap<size> & Bitmap<size>::set(int bit, bool value)
{
    if (bit < size)
    {
        _byte[bit / 8] = value ? (_byte[bit / 8] | (0x80 >> (bit % 8))) : (_byte[bit / 8] & ~(0x80 >> (bit % 8)));
    }

    return *this;
}

template<int size>
int Bitmap<size>::count(bool value)
{
    int count = 0;

    for (int i = 0; i < size; i++)
    {
        if (get(i) == value) count++;
        else return count;
    }
    return count;
}

template<int size>
bool Bitmap<size>::is_full_of(bool value)
{
    for (int i = 0; i < size / 8; i++)
    {
        if (value ? _byte[i] != 0xff : _byte[i] != 0) return false;
    }
    
    return true;
}

template<int size>
Bitmap<size> & Bitmap<size>::reset(bool value)
{
    value ? memset(_byte, 0xff, size / 8) : memset(_byte, 0, size / 8);
    return *this;
}

template<int size>
int Bitmap<size>::allocate(bool value)
{
    auto temp = count(value);
    set(temp, value);
    return temp;
}

#endif /* define: bitmap_h */