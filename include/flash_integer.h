#ifndef _flash_integer_h
#define _flash_integer_h

/**
 * @file	flash_integer.h
 * @author	en2
 * @date	21-04-2020
 * @brief	
 * @details	
**/

#include "flash_bitmap.h"

class Flash_integer
{
public:
    Flash_integer(Flash_sector & sector);
    ~Flash_integer();

    Status restore();

    unsigned int get();
    Status set(unsigned int value);

protected:

private:
    Flash_bitmap _bitmap;

    unsigned int _value;

    static int _address_pointer;
}; /* class: Flash_variable */

#endif /* define: flash_integer_h */