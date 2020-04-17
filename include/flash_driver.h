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
    bool (*read)(int, int, char *);
    bool (*write)(int, int, char *);
    bool (*erase)(int);
}; /* structure: Flash_driver */

#endif /* define: flash_driver_h */