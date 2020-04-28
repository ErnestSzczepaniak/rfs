#ifndef _flash_value_crc_h
#define _flash_value_crc_h

/**
 * @file	flash_value_crc.h
 * @author	en2
 * @date	22-04-2020
 * @brief	
 * @details	
**/

#include "crc.h"
#include "flash_value.h"

template<typename T>
using Crc_generator = unsigned int (*)(T &);

template<typename T, Crc_generator<T> generator>
class Flash_value_crc
{
public:
    T & get();
    Flash_value_crc & set(T value);

    Status load(Flash_sector & sector);
    Status store(Flash_sector & sector);

private:
    Flash_value<T> _value;
    Flash_value<unsigned int> _crc;    

}; /* class: Flash_value_crc */

template<typename T, Crc_generator<T> generator> 
T & Flash_value_crc<T, generator>::get()
{
    return _value.get();
}

template<typename T, Crc_generator<T> generator> 
Flash_value_crc<T, generator> & Flash_value_crc<T, generator>::set(T value)
{
    _value.set(value);
    
    return *this;
}

template<typename T, Crc_generator<T> generator>  
Status Flash_value_crc<T, generator>::load(Flash_sector & sector)
{
    if (auto result_value = _value.load(sector); result_value == false) return result_value;
    if (auto result_crc = _crc.load(sector); result_crc == false) return result_crc;

    if (_crc.get() != generator(_value.get())) return error::frame::Crc_mismatch();

    return true;
}

template<typename T, Crc_generator<T> generator>  
Status Flash_value_crc<T, generator>::store(Flash_sector & sector)
{
    _crc.set(generator(_value.get()));

    if (auto result_value = _value.store(sector); result_value == false) return result_value;
    if (auto result_crc = _crc.store(sector); result_crc == false) return result_crc;

    return true;
}

#endif /* define: Flash_value_crc_h */