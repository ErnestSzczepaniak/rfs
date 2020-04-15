#ifndef _sector_h
#define _sector_h

/**
 * @file	sector.h
 * @author	en2
 * @date	15-04-2020
 * @brief	
 * @details	
**/

#include "flash_driver.h"

class Sector
{
    /**
     * @class	Sector
     * @brief	
     * @details	
    **/

    static constexpr auto size_buffer = 4096;

public:
    Sector(int number, Flash_driver * driver);
    ~Sector();

    bool errase();
    bool read(int offset, int size, unsigned char * to);
    bool write(int offset, int size, unsigned char * from);

protected:
    

private:
    Flash_driver * _driver;

    unsigned int _address;
}; /* class: Sector */


#endif /* define: sector_h */