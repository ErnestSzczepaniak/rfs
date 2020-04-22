#ifndef _rfs_h
#define _rfs_h

/**
 * @file	rfs.h
 * @author	en2
 * @date	21-04-2020
 * @brief	
 * @details	
**/

constexpr auto size_sector =        512;
constexpr auto number_sectors =     2;
constexpr auto size_flash = size_sector * number_sectors;

constexpr auto size_bitmap =        4;
constexpr auto number_bits = size_bitmap * 8;

struct Rfs_driver
{
    bool (*read)(int, int, char *);
    bool (*write)(int, int, char *);
    bool (*clear)(int, int);
}; /* structure: Flash_driver */


#endif /* define: rfs_h */