#ifndef _flash_value_filesystem_h
#define _flash_value_filesystem_h

/**
 * @file	flash_value_filesystem.h
 * @author	en2
 * @date	28-04-2020
 * @brief	
 * @details	
**/

#include "flash_value_crc.h"
#include "flash_bitmap.h"

template<typename T, CRC<T> crc, int bits = 8>
class Flash_value_filesystem
{
    using Value = Flash_value_crc<T, crc>;
    using Bitmap = Flash_value<Flash_bitmap<bits>>;

public:
    T & get();
    Flash_value_filesystem & set(T value);

    Status load(Flash_sector & sector);
    Status store(Flash_sector & sector);

    Status recover(Flash_sector & sector);

    int usage();

protected:
    Result<int> _allocate(Flash_sector & sector);

private:
    Bitmap _bitmap;
    Value _value;

}; /* class: Flash_value_filesystem */

template<typename T, CRC<T> crc, int bits>
T & Flash_value_filesystem<T, crc, bits>::get()
{
    return _value.get();
}

template<typename T, CRC<T> crc, int bits>
Flash_value_filesystem<T, crc, bits> & Flash_value_filesystem<T, crc, bits>::set(T value)
{
    _value.set(value);

    return *this;
}

template<typename T, CRC<T> crc, int bits>
Status Flash_value_filesystem<T, crc, bits>::load(Flash_sector & sector)
{
    if (auto result_bitmap = _bitmap.load(sector); result_bitmap == false) return result_bitmap;
    
    if (_bitmap.get().is_full(true)) return true;

    auto offset = sizeof(Bitmap) + (usage() - 1) * sizeof(Value);

    if (auto result_value = _value.load(sector.at(offset)); result_value == false) return result_value;

    return true;
}

template<typename T, CRC<T> crc, int bits>
Status Flash_value_filesystem<T, crc, bits>::store(Flash_sector & sector)
{
    auto base_address = sector.at();

    if (auto result_bitmap = _bitmap.load(sector); result_bitmap == false) return result_bitmap;

    if (_bitmap.get().is_full(false))
    {
        if (auto result_clear = sector.at(base_address).clear(sizeof(Bitmap) + bits * sizeof(Value)); result_clear == false) return result_clear;
        _bitmap.get().fill(true);
    }

    auto [status_allocate, next_index] = _allocate(sector.at(base_address));

    auto offset = sizeof(Bitmap) + next_index * sizeof(Value);

    return _value.store(sector.at(base_address + offset));
}

template<typename T, CRC<T> crc, int bits>
Status Flash_value_filesystem<T, crc, bits>::recover(Flash_sector & sector)
{
    auto base_address = sector.at();

    if (usage() > 1)
    {
        auto offset = sizeof(Bitmap) + (usage() - 2) * sizeof(Value);

        if (auto status_load = _value.load(sector.at(base_address + offset)); status_load == false) return status_load;

        sector.at(base_address);

        return store(sector);
    }
    else return false;
}

template<typename T, CRC<T> crc, int bits>
int Flash_value_filesystem<T, crc, bits>::usage()
{
    return _bitmap.get().count(false);
}

/* ---------------------------------------------| info |--------------------------------------------- */

template<typename T, CRC<T> crc, int bits>
Result<int> Flash_value_filesystem<T, crc, bits>::_allocate(Flash_sector & sector)
{
    Result<int> result;

    auto index = _bitmap.get().count(false);
    _bitmap.get().set(index, false);

    if (auto status = _bitmap.store(sector); status == false) return status;

    result.value = index;

    return result;
}

#endif /* define: flash_value_filesystem_h */