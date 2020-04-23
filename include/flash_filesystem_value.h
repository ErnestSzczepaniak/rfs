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
    static constexpr auto size_total = sizeof(Flash_bitmap) + number_bits * sizeof(Flash_crc_value<T>);

public:
    T & get();
    Flash_filesystem_value<T> & set(T value);

    Status load_from(Flash_sector & sector);
    Status store_to(Flash_sector & sector);
    Status recover_from(Flash_sector & sector);

    int usage();

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
    if (_bitmap.get().is_full_of(true)) return warning::value::Empty();

    auto offset = (usage() - 1) * sizeof(Flash_crc_value<T>);

    if (auto result_value = _value.load_from(sector.at_offset(offset)); result_value == false) return result_value;

    return true;
}

template<typename T>
Status Flash_filesystem_value<T>::store_to(Flash_sector & sector)
{
    auto base_address = sector.at();

    if (auto result_bitmap = _bitmap.load_from(sector); result_bitmap == false) return result_bitmap;

    if (_bitmap.get().is_full_of(false))
    {
        if (auto result_clear = sector.at(base_address).clear(size_total); result_clear == false) return result_clear;
        _bitmap.get().reset(true);
    }

    auto offset = _bitmap.get().allocate(false) * sizeof(Flash_crc_value<T>);

    if (auto result_bitmap = _bitmap.store_to(sector.at(base_address)); result_bitmap == false) return result_bitmap;
    if (auto result_value = _value.store_to(sector.at_offset(offset)); result_value == false) return result_value;

    return true;
}

template<typename T>
Status Flash_filesystem_value<T>::recover_from(Flash_sector & sector)
{
    if (usage() > 1)
    {
        sector.at_offset(sizeof(Flash_bitmap)); // align pointers as in load_from

        auto offset = (usage() - 2) * sizeof(Flash_crc_value<T>);
        if (auto result_value = _value.load_from(sector.at_offset(offset)); result_value == false) return result_value;

        return true;
    }
    else return false;
}

template<typename T>
int Flash_filesystem_value<T>::usage()
{
    return _bitmap.get().count(false);
}

#endif /* define: flash_filesystem_value_h */