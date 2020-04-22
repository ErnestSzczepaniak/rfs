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
    Flash_sector();
    ~Flash_sector();    

    Flash_sector & bind(int number, Rfs_driver * driver);

    Status clear(int address = 0, int range = size_sector);

    template<typename T> Result<T> read(int address);
    template<typename T> Status write(int address, T & value);
    template<typename T> Result<bool> write_back(int address, T & value);
    
    template<typename T> Result<bool> verify(int address, T & value);
    template<typename T> Status replace(int address, T & value);

private:
    int _number;
    Rfs_driver * _driver;

}; /* class: Flash_sector */

template<typename T>
Result<T> Flash_sector::read(int address)
{
    Result<T> result;

    if (address + sizeof(T) > size_sector) return error::argument::Out_of_range();
    if (_driver == nullptr) return error::driver::Not_present();
    if (_driver->read(_number * size_sector + address, sizeof(T), (char *)&result.value) == false) return error::driver::Read();
 
    return result;
}

template<typename T>
Status Flash_sector::write(int address, T & value)
{
    if (address + sizeof(T) > size_sector) return error::argument::Size_mismatch();
    if (_driver == nullptr) return error::driver::Not_present();
    if (_driver->write(_number * size_sector + address, sizeof(T), (char *)&value) == false) return error::driver::Write();
 
    return true;
}

template<typename T>
Result<bool> Flash_sector::write_back(int address, T & value)
{
    Result<bool> result;

    auto status_write = write(address, value);
    if (status_write == false) return status_write;

    auto [status_verify, success] = verify(address, value);
    if (status_verify == false) return status_verify;

    result.value = success;

    return result;
}

template<typename T>
Result<bool> Flash_sector::verify(int address, T & value)
{
    Result<bool> result;

    auto result_read = read<T>(address);
    if (result_read.status == false) return result_read.status;

    result.value = memcmp(&value, &result_read.value, sizeof(T)) == 0 ? true : false;

    return result;
}

template<typename T>
Status Flash_sector::replace(int address, T & value)
{
    auto status_clear = clear(address, sizeof(T));
    if (status_clear == false) return status_clear;

    return write(address, value);
}

#endif /* define: flash_Flash_sector_h */