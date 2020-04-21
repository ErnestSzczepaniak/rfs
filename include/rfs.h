#ifndef _rfs_h
#define _rfs_h

/**
 * @file	rfs.h
 * @author	en2
 * @date	21-04-2020
 * @brief	
 * @details	
**/

constexpr auto size_sector =        256;
constexpr auto number_sectors =     4;
constexpr auto size_bitmap =        4;

constexpr auto size_flash = size_sector * number_sectors;
constexpr auto number_bits = size_bitmap * 8;

struct Sector
{
    unsigned char payload[size_sector];
}; /* structure: Sector */

static_assert(sizeof(Sector) == size_sector);

struct Bitmap
{
    unsigned char payload[size_bitmap];
}; /* structure: Bitmap */

static_assert(sizeof(Bitmap) == size_bitmap);

struct Rfs_driver
{
    bool (*read)(int, int, char *);
    bool (*write)(int, int, char *);
    bool (*erase)(int);
}; /* structure: Flash_driver */

#endif /* define: rfs_h */