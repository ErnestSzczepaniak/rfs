#ifndef _flash_driver_generic_h
#define _flash_driver_generic_h

/**
 * @file	flash_driver_generic.h
 * @author	en2
 * @date	28-04-2020
 * @brief	
 * @details	
**/

class Flash_driver_generic
{
    /**
     * @class	Flash_driver
     * @brief	
     * @details	
    **/

    static constexpr auto _size_sector = 256 * 1024;
    static constexpr auto _number_sector = 5;
    static constexpr auto _number_bits = 1024 * 8;
    static constexpr auto _size_flash = _size_sector * _number_sector;

public:
    virtual bool init() = 0;
    virtual bool deinit() = 0;

    virtual bool read(int, int, unsigned char *) = 0;
    virtual bool write(int, int, unsigned char *) = 0;
    virtual bool erase(int, int) = 0;

    static constexpr int size_sector() {return _size_sector;}
    static constexpr int number_sector() {return _number_sector;}
    static constexpr int number_bits() {return _number_bits;}
    static constexpr int size_flash() {return _size_flash;}

}; /* class: Flash_driver_generic */

#endif /* define: flash_driver_generic_h */