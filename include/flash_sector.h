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
    Flash_sector(int number, Rfs_driver * driver);
    ~Flash_sector();

    Flash_sector & number(int value);
    int number();

    Flash_sector & at(int value);
    int at();

    Status reset();
    Status clear(int value);

    Result<Sector> read();
    Status write(Sector & sector);

    Result<bool> verify_with(Flash_sector & other);
    Status replace_with(Flash_sector & other);

    template<typename T> Result<T> read_value();
    template<typename T> Status write_value(T & value);
    template<typename T> Result<bool> verify_with_value(T & value);
    template<typename T> Status replace_with_value(T & value);

private:
    Rfs_driver * _driver;

    int _number;
    int _at;
}; /* class: Flash_sector */

template<typename T>
Result<T> Flash_sector::read_value()
{
    Result<T> result;

    if (_at + sizeof(T) > size_sector) return error::argument::Size_mismatch();
    if (_driver->read(_number * size_sector + _at, sizeof(T), (char *)&result.value) == false) return error::driver::Read();
 
    _at += sizeof(T);

    return result;
}

template<typename T>
Status Flash_sector::write_value(T & value)
{
    if (_at + sizeof(T) > size_sector) return error::argument::Size_mismatch();
    if (_driver->write(_number * size_sector + _at, sizeof(T), (char *)&value) == false) return error::driver::Write();
 
    _at += sizeof(T);

    return true;
}

template<typename T>
Result<bool> Flash_sector::verify_with_value(T & value)
{
    Result<bool> result;

    if (_at + sizeof(T) > size_sector) return error::argument::Size_mismatch();

    auto [status_read, buffer] = read_value<T>();

    if (status_read == false) return status_read;

    auto * ptr1 = (char *)&value;
    auto * ptr2 = (char *)&buffer;

    result.value = memcmp(&value, &buffer, sizeof(T)) == 0 ? true : false;

    return result;
}

template<typename T>
Status Flash_sector::replace_with_value(T & value)
{
    Sector sector;

    if (_at + sizeof(T) > size_sector) return error::argument::Size_mismatch();
    if (_driver->read(_number * size_sector, size_sector, (char *)&sector.payload[0]) == false) return error::driver::Read();
    if (auto status_reset = reset(); status_reset == false) return status_reset;

    memcpy(&sector.payload[_at], &value, sizeof(T));

    return at(0).write_value(sector);
}

#endif /* define: flash_Flash_sector_h */