#ifndef _flash_Flash_sector_h
#define _flash_Flash_sector_h

/**
 * @file	flash_Flash_sector.h
 * @author	en2
 * @date	17-04-2020
 * @brief	
 * @details	
**/

#include "flash_driver_generic.h"
#include "result.h"
#include "string.h"

class Flash_sector
{
public:
    Flash_sector(int number = 0, Flash_driver_generic * driver = nullptr);
    ~Flash_sector();

    Flash_sector & number(int number);
    int number();

    Flash_sector & address(int address);
    int address();

    Flash_sector & driver(Flash_driver_generic * driver);
    Flash_driver_generic * driver();

    Flash_sector & at(int value);
    int at();

    Result<bool> is_empty();
    Result<bool> is_full();

    Status clear(int range = 0);

    template<typename T> Result<T> read();
    template<typename T> Status write(T & value);

private:
    int _at;
    int _number;
    Flash_driver_generic * _driver;

}; /* class: Flash_sector */

template<typename T>
Result<T> Flash_sector::read()
{
    Result<T> result;

    if (_at + sizeof(T) > Flash_driver_generic::size_sector()) return status::error::argument::Out_of_range();
    if (_driver == nullptr) return status::error::driver::Not_present();
    if (_driver->read(_number * Flash_driver_generic::size_sector() + _at, sizeof(T), (unsigned char *) &result.value) == false) return status::error::driver::Read();
 
    _at += sizeof(T);

    return result;
}

template<typename T>
Status Flash_sector::write(T & value)
{
    if (_at + sizeof(T) > Flash_driver_generic::size_sector()) return status::error::argument::Size_mismatch();
    if (_driver == nullptr) return status::error::driver::Not_present();    
    if (_driver->write(_number * Flash_driver_generic::size_sector() + _at, sizeof(T), (unsigned char *) &value) == false) return status::error::driver::Write();
 
    _at += sizeof(T);

    return true;
}


#endif /* define: flash_Flash_sector_h */