#ifndef _flash_filesystem_value_h
#define _flash_filesystem_value_h

/**
 * @file	flash_filesystem_value.h
 * @author	en2
 * @date	22-04-2020
 * @brief	
 * @details	
**/

#include "flash_crc_value.h"
#include "bitmap.h"

template<typename T>
class Flash_filesystem_value
{
    using Flash_bitmap = Bitmap<number_bits>;

public:
    T & get();
    Flash_filesystem_value<T> & set(T value);

    Status load_from(Flash_sector & sector);
    Status store_to(Flash_sector & sector);

private:
    Flash_value<Flash_bitmap> _bitmap;
    Flash_crc_value<T> _value;

}; /* class: Flash_filesystem_value */

template<typename T>
T & Flash_filesystem_value<T>::get()
{
    return _value.get();
}

template<typename T>
Flash_filesystem_value<T> & Flash_filesystem_value<T>::set(T value)
{
    _value.set(value);

    return *this;
}

template<typename T>
Status Flash_filesystem_value<T>::load_from(Flash_sector & sector)
{
    if (auto result_bitmap = _bitmap.load_from(sector); result_bitmap == false) return result_bitmap;
    if (_bitmap.get().is_full_of(true) == true) return true;

    auto offset = (_bitmap.get().count(false) - 1) * sizeof(Flash_crc_value<T>);

    if (auto result_value = _value.load_from(sector.at(sector.at() + offset)); result_value == false) return result_value;

    return true;
}

template<typename T>
Status Flash_filesystem_value<T>::store_to(Flash_sector & sector)
{
    if (auto index = _bitmap.get().allocate(false); index == number_bits) _bitmap.get().reset(false);

    auto offset = (_bitmap.get().count(false) - 1) * sizeof(Flash_crc_value<T>);

    if (auto result_bitmap = _bitmap.store_to(sector); result_bitmap == false) return result_bitmap;
    if (auto result_value = _value.store_to(sector.at(sector.at() + offset)); result_value == false) return result_value;

    return true;
}

#endif /* define: flash_filesystem_value_h */