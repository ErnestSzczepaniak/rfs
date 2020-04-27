#ifndef _flash_Flash_sector_h
#define _flash_Flash_sector_h

/**
 * @file	flash_Flash_sector.h
 * @author	en2
 * @date	17-04-2020
 * @brief	
 * @details	
**/

#include "rfs.h"
#include "result.h"
#include "string.h"

class Flash_sector
{
public:
    Flash_sector & number(int value);
    int number();
    int address();

    Flash_sector & driver(Rfs_driver * driver);
    Rfs_driver * driver();

    Flash_sector & at(int value);
    Flash_sector & at_offset(int value);
    int at();

    Result<bool> is_empty();
    Status clear(int range = size_sector);
    template<typename T> Result<T> read();
    template<typename T> Status write(T & value);

private:
    int _number;
    Rfs_driver * _driver;
    int _at;

}; /* class: Flash_sector */

template<typename T>
Result<T> Flash_sector::read()
{
    Result<T> result;

    if (_at + sizeof(T) > size_sector) return error::argument::Out_of_range();
    if (_driver == nullptr) return error::driver::Not_present();
    if (_driver->read(_number * size_sector + _at, sizeof(T), (char *)&result.value) == false) return error::driver::Read();
 
    _at += sizeof(T);

    return result;
}

template<typename T>
Status Flash_sector::write(T & value)
{
    if (_at + sizeof(T) > size_sector) return error::argument::Size_mismatch();
    if (_driver == nullptr) return error::driver::Not_present();
    if (_driver->write(_number * size_sector + _at, sizeof(T), (char *)&value) == false) return error::driver::Write();
 
    _at += sizeof(T);

    return true;
}


#endif /* define: flash_Flash_sector_h */