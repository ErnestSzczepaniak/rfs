#include "flash_bitmap.h"

Flash_bitmap::Flash_bitmap()
{

}

Flash_bitmap::~Flash_bitmap()
{

}

bool Flash_bitmap::is_empty()
{
    return (_bitmap[0] == 0xff);
}

bool Flash_bitmap::is_full()
{
    return (_bitmap[size_bitmap - 1] == 0);
}

int Flash_bitmap::allocate()
{
    auto busy = _busy();

    auto byte = busy / 8;
    auto mask = (0x80 >> (busy % 8));
    
    _bitmap[byte] &= ~mask;

    return busy;
}

/* ---------------------------------------------| info |--------------------------------------------- */

int Flash_bitmap::_busy()
{
    int counter = 0;

    for (auto & byte : _bitmap)
    {
        for (int i = 0; i < 8; i++)
        {
            if ((byte & (0x80 >> i)) == 0) counter++;
            else return counter;
        }   
    }

    return counter;
}

