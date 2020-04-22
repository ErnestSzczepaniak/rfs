#ifndef _flash_bitmap_h
#define _flash_bitmap_h

/**
 * @file	flash_bitmap.h
 * @author	en2
 * @date	22-04-2020
 * @brief	
 * @details	
**/

#include "rfs.h"

class Flash_bitmap
{
public:
    Flash_bitmap();
    ~Flash_bitmap();

    bool is_empty();
    bool is_full();

    int allocate();

    unsigned char _bitmap[size_bitmap];
    int _busy();
protected:

private:

}; /* class: Flash_bitmap */


#endif /* define: flash_bitmap_h */