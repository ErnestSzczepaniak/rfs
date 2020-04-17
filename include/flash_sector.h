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

class Flash_sector
{
    /**
     * @class	Flash_sector
     * @brief	
     * @details	
    **/

   static constexpr auto size = 64;

public:
    Flash_sector(int number, Flash_driver * driver);
    ~Flash_sector();

    Flash_sector & number(int value);
    int number();

    Flash_sector & at(int value);
    int at();

    bool reset();
    int clear(int value);

    template<typename T> int read_to(T & value);
    template<typename T> int write_from(T & value);
    template<typename T> int verify_with(T & value);
    template<typename T> int replace_with(T & value);

protected:
    template<typename T> int _space(T & payload);

private:
    Flash_driver * _driver;

    int _number;
    int _at;
}; /* class: Flash_sector */

template<typename T>
int Flash_sector::read_to(T & value)
{
    auto space = _space(value);

    if (_driver->read(_number * size + _at, space, (char *)&value) == false) return -1;
 
    _at += space;

    return space;
}

template<typename T>
int Flash_sector::write_from(T & value)
{
    auto space = _space(value);

    if (_driver->write(_number * size + _at, space, (char *)&value) == false) return -1;
 
    _at += space;

    return space;
}

template<typename T>
int Flash_sector::verify_with(T & value)
{
    if (_space(value) < sizeof(T)) return -1;

    T buffer;

    if (read_to(buffer) != sizeof(T)) return -1;

    auto * ptr1 = (char *)&value;
    auto * ptr2 = (char *)&buffer;

    for (int i = 0; i < sizeof(T); i++)
    {
        if (ptr1[i] != ptr2[i]) return i;
    }
    
    return 0;
}

template<typename T>
int Flash_sector::replace_with(T & value)
{
    auto space = _space(value);

    char buffer[size];
    auto stored = _at;

    if (at(0).read_to(buffer) != size) return -1;
    if (reset() == false) return -1;

    memcpy(&buffer[stored], &value, space);
    
    return at(0).write_from(buffer) == size ? space : -1;
}

/* ---------------------------------------------| info |--------------------------------------------- */

template<typename T>
int Flash_sector::_space(T & value)
{
    return (_at + sizeof(T) >= size) ? size - _at : sizeof(T);
}

#endif /* define: flash_Flash_sector_h */