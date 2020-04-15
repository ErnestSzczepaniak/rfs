#ifndef _flash_driver_h
#define _flash_driver_h

/**
 * @file	flash_driver.h
 * @author	en2
 * @date	15-04-2020
 * @brief	
 * @details	
**/

struct Flash_driver
{
    bool (*read)(unsigned int, int, unsigned int *);
    bool (*write)(unsigned int, int, unsigned int *);
    bool (*erase)(unsigned int);
    int size_sector;
    int size_total;
}; /* structure: Flash_driver */

#endif /* define: flash_driver_h */