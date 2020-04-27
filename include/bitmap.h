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

template<int size, bool initial = true>
class Bitmap
{   
    static_assert(size % 8 == 0);
    static constexpr unsigned char _mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

public:
    bool get(int bit);
    Bitmap & set(int bit);

    int count_set();
    int count_not_set();

    bool is_empty();
    bool is_full();

    Bitmap & reset();

private:
    unsigned char _byte[size / 8];

}; /* class: Flash_bitmap */

template<int size, bool initial>
bool Bitmap<size, initial>::get(int bit)
{
    return (_byte[bit / 8] & _mask[bit % 8]);
}

template<int size, bool initial>
Bitmap<size, initial> & Bitmap<size, initial>::set(int bit)
{
    if constexpr (initial == true)
    {
        _byte[bit / 8] &= ~_mask[bit % 8];
    } 
    else if constexpr (initial == false)
    {
        _byte[bit / 8] |= _mask[bit % 8];
    }

    return *this;
}

template<int size, bool initial>
int Bitmap<size, initial>::count_set()
{
    int count = 0;

    for (int i = 0; i < size; i++)
    {
        if constexpr (initial == true) 
        {
            if (get(i) == false) count++;
        }
        else if constexpr (initial == false)
        {
            if (get(i) == true) count++;
        }
    }

    return count;
}

template<int size, bool initial>
bool Bitmap<size, initial>::is_full()
{
    return (count_set() == size);
}

template<int size, bool initial>
bool Bitmap<size, initial>::is_empty()
{
    return (count_set() == 0);
}

template<int size, bool initial>
Bitmap<size, initial> & Bitmap<size, initial>::reset()
{
    if constexpr (initial == true)
    {
        memset(_byte, 0xff, size / 8);
    }  
    else if constexpr (initial == false)
    {
        memset(_byte, 0, size / 8);
    }

    return *this;
}



#endif /* define: bitmap_h */