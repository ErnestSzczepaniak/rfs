#ifndef _flash_value_safe_h
#define _flash_value_safe_h

/**
 * @file	flash_value_safe.h
 * @author	en2
 * @date	23-04-2020
 * @brief	
 * @details	
**/

#include "flash_value_filesystem.h"

template<typename T, CRC<T> crc, int bits>
class Flash_value_safe
{
    using Value = Flash_value_filesystem<T, crc, bits>;

public:
    Flash_value_safe(int address, Flash_sector & primary, Flash_sector & secondary);
    ~Flash_value_safe();

    T & get();
    Flash_value_safe & set(T value);

    Status load();
    Status store();

protected:
    Status _copy_primary();
    Status _copy_secondary();
    Status _recover_primary();
    Status _recover_secondary();

private:
    int _address;

    Flash_sector & _sector_primary;
    Flash_sector & _sector_secondary;

    Value _value_primary;
    Value _value_secondary;

}; /* class: Flash_value_safe */

template<typename T, CRC<T> crc, int bits> 
Flash_value_safe<T, crc, bits>::Flash_value_safe(int address, Flash_sector & primary, Flash_sector & secondary)
:
_address(address),
_sector_primary(primary),
_sector_secondary(secondary)
{

}

template<typename T, CRC<T> crc, int bits> 
Flash_value_safe<T, crc, bits>::~Flash_value_safe()
{

}

template<typename T, CRC<T> crc, int bits> 
T & Flash_value_safe<T, crc, bits>::get()
{   
    return _value_primary.get();
}

template<typename T, CRC<T> crc, int bits> 
Flash_value_safe<T, crc, bits> & Flash_value_safe<T, crc, bits>::set(T value)
{
    _value_primary.set(value);
    _value_secondary.set(value);

    return *this;
}

template<typename T, CRC<T> crc, int bits> 
Status Flash_value_safe<T, crc, bits>::load()
{
    auto status_primary = _value_primary.load(_sector_primary.at(_address));
    auto status_secondary = _value_secondary.load(_sector_secondary.at(_address));

    if (status_primary == true && status_secondary == true)
    {
        if (_value_primary.usage() > 0 && _value_secondary.usage() > 0)
        {
            if (_value_primary.get() != _value_secondary.get()) return _copy_primary();
            else return true;
        }
        else if (_value_primary.usage() == 0 && _value_secondary.usage() > 0) return _copy_secondary();
        else if (_value_primary.usage() > 0 && _value_primary.usage() == 0) return _copy_primary();
        else return warning::value::Empty();
    }
    else if (status_primary == true && status_secondary == error::frame::Crc_mismatch()) // seconday 
    {
        if (_value_primary.usage() > 0) return _copy_primary();
        else return _recover_secondary();
    }
    else if (status_primary == error::frame::Crc_mismatch() && status_secondary == true)
    {
        if (_value_secondary.usage() > 0) return _copy_secondary();
        else return _recover_primary();
    }
    else if (status_primary == error::frame::Crc_mismatch() && status_secondary == error::frame::Crc_mismatch())
    {
        if (_value_primary.usage() > _value_secondary.usage()) return _recover_primary();
        else return _recover_secondary();
    }

    return false;
}

template<typename T, CRC<T> crc, int bits> 
Status Flash_value_safe<T, crc, bits>::store()
{
    if (auto status_primary = _value_primary.store(_sector_primary.at(_address)); status_primary == false) return status_primary;
    if (auto status_secondary = _value_secondary.store(_sector_secondary.at(_address)); status_secondary == false) return status_secondary;

    return true;
}

/* ---------------------------------------------| info |--------------------------------------------- */

template<typename T, CRC<T> crc, int bits> 
Status Flash_value_safe<T, crc, bits>::_copy_primary()
{
    _value_secondary.set(_value_primary.get());

    return _value_secondary.store(_sector_secondary.at(_address));
}

template<typename T, CRC<T> crc, int bits> 
Status Flash_value_safe<T, crc, bits>::_copy_secondary()
{
    _value_primary.set(_value_secondary.get());

    return _value_primary.store(_sector_primary.at(_address));
}

template<typename T, CRC<T> crc, int bits> 
Status Flash_value_safe<T, crc, bits>::_recover_primary()
{
    if (auto status_recover = _value_primary.recover(_sector_primary.at(_address)); status_recover == false) return status_recover;

    _value_secondary.set(_value_primary.get());

    return _value_secondary.store(_sector_secondary.at(_address)); 
}

template<typename T, CRC<T> crc, int bits> 
Status Flash_value_safe<T, crc, bits>::_recover_secondary()
{
    if (auto status_recover = _value_secondary.recover(_sector_secondary.at(_address)); status_recover == false) return status_recover;

    _value_primary.set(_value_secondary.get());
    
    return _value_primary.store(_sector_primary.at(_address));
}

#endif /* define: Flash_value_safe_h */