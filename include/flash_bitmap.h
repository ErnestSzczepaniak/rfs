#ifndef _flash_bitmap_h
#define _flash_bitmap_h

/**
 * @file	flash_bitmap.h
 * @author	en2
 * @date	17-04-2020
 * @brief	
 * @details	
**/

#include "flash_sector.h"

class Flash_bitmap
{
public:
    Flash_bitmap(int address, Flash_sector & sector);
    ~Flash_bitmap();

    Flash_bitmap & address(int value);
    int address();

    Flash_bitmap & sector(Flash_sector & value);
    Flash_sector & sector();

    Status reset();

    Status clear(int bit);
    Result<bool> is_cleared(int bit);
    Result<int> count_cleared();

private:
    Flash_sector & _sector;
    int _address;

}; /* class: Flash_bitmap */


#endif /* define: flash_bitmap_h */