#ifndef _flash_Flash_sector_h
#define _flash_Flash_sector_h

/**
 * @file	flash_Flash_sector.h
 * @author	en2
 * @date	17-04-2020
 * @brief	
 * @details	
**/

#include "flash_driver.h"
#include "string.h"
#include "result.h"

class Flash_sector
{
    /**
     * @class	Flash_sector
     * @brief	
     * @details	
    **/

    static constexpr auto size = 64;
    struct Sector{unsigned char payload[64];};

public:
    Flash_sector(int number, Flash_driver * driver);
    ~Flash_sector();

    Flash_sector & number(int value);
    int number();

    Flash_sector & at(int value);
    int at();

    Status reset();
    Status clear(int value);

    template<typename T> Status write(T & value);
    template<typename T> Status replace_with(T & value);

    template<typename T> Result<bool> verify_with(T & value);
    template<typename T> Result<T> read();

protected:
    template<typename T> int _space(T & payload);

private:
    Flash_driver * _driver;

    int _number;
    int _at;
}; /* class: Flash_sector */

template<typename T>
Result<T> Flash_sector::read()
{
    Result<T> result;

    if (_at + sizeof(T) > size) return Argument::Out_of_range();

    if (_driver->read(_number * size + _at, sizeof(T), (char *)&result.value) == false) return Driver::Read_error();
 
    _at += sizeof(T);

    return result;
}

template<typename T>
Status Flash_sector::write(T & value)
{
    if (_at + sizeof(T) > size) return Argument::Out_of_range();

    if (_driver->write(_number * size + _at, sizeof(T), (char *)&value) == false) Driver::Write_error();
 
    _at += sizeof(T);

    return true;
}

template<typename T>
Result<bool> Flash_sector::verify_with(T & value)
{
    Result<bool> result;

    if (_at + sizeof(T) > size) return Argument::Out_of_range();

    auto [status, buffer] = read<T>();

    if (status == false) return status;

    auto * ptr1 = (char *)&value;
    auto * ptr2 = (char *)&buffer;

    result.value = true;

    for (int i = 0; i < sizeof(T); i++)
    {
        if (ptr1[i] != ptr2[i])
        {
            result.value = false;
            break;
        }
    }

    return result;
}

template<typename T>
Status Flash_sector::replace_with(T & value)
{
    if (_at + sizeof(T) > size) return Argument::Out_of_range();

    auto stored = _at;

    auto [status_read, sector] = read<Sector>();

    if (status_read == false) return status_read;

    if (auto status_reset = reset(); status_reset == false) return status_reset;

    memcpy(&sector.payload[stored], &value, sizeof(T));
    
    return at(0).write(sector);
}

/* ---------------------------------------------| info |--------------------------------------------- */

template<typename T>
int Flash_sector::_space(T & value)
{
    return (_at + sizeof(T) >= size) ? size - _at : sizeof(T);
}

#endif /* define: flash_Flash_sector_h */