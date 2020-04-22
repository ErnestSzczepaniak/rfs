#ifndef _flash_crc_value_h
#define _flash_crc_value_h

/**
 * @file	flash_crc_value.h
 * @author	en2
 * @date	22-04-2020
 * @brief	
 * @details	
**/

#include "crc.h"
#include "flash_value.h"

template<typename T>
class Flash_crc_value
{
public:
    Flash_crc_value();
    ~Flash_crc_value();

    T & get();
    Flash_crc_value & set(T value);

    Status load(int address, Flash_sector & sector);
    Status store(int address, Flash_sector & sector);

private:
    Flash_value<T> _value;
    Flash_value<unsigned int> _crc;    

}; /* class: Flash_crc_value */

template<typename T> 
Flash_crc_value<T>::Flash_crc_value()
{

}

template<typename T> 
Flash_crc_value<T>::~Flash_crc_value()
{

}

template<typename T> 
T & Flash_crc_value<T>::get()
{
    return _value.get();
}

template<typename T>
Flash_crc_value<T> & Flash_crc_value<T>::set(T value)
{
    _value.set(value);
    _crc.set(crc<32>(value));

    return *this;
}

template<typename T> 
Status Flash_crc_value<T>::load(int address, Flash_sector & sector)
{
    if (auto result_value = _value.load(address, sector); result_value == false) return result_value;
    if (auto result_crc = _crc.load(address + sizeof(T), sector); result_crc == false) return result_crc;

    if (_crc.get() != crc<32>(_value.get())) return error::frame::Crc_mismatch();

    return true;
}

template<typename T> 
Status Flash_crc_value<T>::store(int address, Flash_sector & sector)
{
    if (auto result_value = _value.store(address, sector); result_value == false) return result_value;
    if (auto result_crc = _crc.store(address + sizeof(T), sector); result_crc == false) return result_crc;
}

#endif /* define: flash_crc_value_h */