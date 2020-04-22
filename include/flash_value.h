#ifndef _flash_integer_h
#define _flash_integer_h

/**
 * @file	flash_integer.h
 * @author	en2
 * @date	22-04-2020
 * @brief	
 * @details	
**/

#include "flash_sector.h"

template<typename T>
class Flash_value
{
public:
    Flash_value();
    ~Flash_value();

    T & get();
    Flash_value & set(T value);

    Status load(int address, Flash_sector & sector);
    Status store(int address, Flash_sector & sector);

private:
    T _value;
};

template<typename T> 
Flash_value<T>::Flash_value()
{

}

template<typename T> 
Flash_value<T>::~Flash_value()
{

}

template<typename T> 
T & Flash_value<T>::get()
{
    return _value;
}

template<typename T>
Flash_value<T> & Flash_value<T>::set(T value)
{
    _value = value;

    return *this;
}

template<typename T> 
Status Flash_value<T>::load(int address, Flash_sector & sector)
{
    auto result = sector.read<T>(address);
    if (result.status == false) return result.status;

    _value = result.value;

    return true;
}

template<typename T> 
Status Flash_value<T>::store(int address, Flash_sector & sector)
{
    if (auto status_write = sector.write(address, _value); status_write == false) return status_write;

    return true;
}


#endif /* define: flash_integer_h */