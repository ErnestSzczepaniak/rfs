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
    /**
     * @class	Flash_bitmap
     * @brief	
     * @details	
    **/
public:
    Flash_bitmap(int address, int width, Flash_sector * sector);
    ~Flash_bitmap();

    bool reset();

    bool set(int bit);
    bool get(int bit);

private:
    int _address;
    int _width;

}; /* class: Flash_bitmap */


#endif /* define: flash_bitmap_h */