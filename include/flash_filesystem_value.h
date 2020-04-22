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
#include "flash_bitmap.h"

template<typename T>
class Flash_filesystem_value
{
public:
    Flash_filesystem_value();
    ~Flash_filesystem_value();

    T & value();
    Status restore(int address, Flash_sector & sector);
    Status update(int address, Flash_sector & sector, T value);

protected:
    // Status _bitmap_update(int address, Flash_sector & sector, int bit)
    // {
    //     if (bit > number_bits || bit < 0) return error::argument::Out_of_range();
    //     else if (bit < number_bits)
    //     {
    //         auto offset = bit / 8;
    //         unsigned char mask = (0x80 >> (bit % 8));

    //         //_bitmap.value().byte[offset] &= ~mask;
    //     }
    //     else
    //     {
    //         memset(&_bitmap.value(), 0xff, size_bitmap);
    //         //_bitmap.value().byte[0] &= ~0x80;
    //     }

    //     return _bitmap.update(address, sector, _bitmap.value());
    // }

private:
    Flash_value<Flash_bitmap> _bitmap;
    Flash_crc_value<T> _value;

}; /* class: Flash_filesystem_value */

template<typename T>
Flash_filesystem_value<T>::Flash_filesystem_value()
{

}

template<typename T>
Flash_filesystem_value<T>::~Flash_filesystem_value()
{

}

template<typename T>
Status Flash_filesystem_value<T>::restore(int address, Flash_sector & sector)
{
    // if (auto status_bitmap = _bitmap.restore(address, sector); status_bitmap == false) return status_bitmap;

    // if (_bitmap.value().is_empty() == true) return true;

    // auto offset = (_bitmap.value().busy() - 1) * (sizeof(Flash_crc_value<T>));

    // return _value.restore(address + offset, sector);
}

template<typename T>
T & Flash_filesystem_value<T>::value()
{
    //return _value.value();
}

template<typename T>
Status Flash_filesystem_value<T>::update(int address, Flash_sector & sector, T value)
{
    // if (auto status_bitmap = _bitmap.restore(address, sector); status_bitmap == false) return status_bitmap;

    // if (_bitmap.value().is_full() == true)
    // {

    // }
    // else
    // {
        
    // }
    
    // auto next = _bitmap_allocate(_bitmap.value());
    // auto offset = size_bitmap + next * sizeof(Flash_crc_value<T>);

    // if (next == 0) // restart fragment
    // {

    // }
    // else
    // {
    //     if (auto status_bitmap = _bitmap.update(address, sector, _bitmap.value()); status_bitmap == false) return status_bitmap;
    //     if (auto status_value = _value.update(address + offset, sector, value); status_value == false) return status_value;
    // }
    
    return true;
}

#endif /* define: flash_filesystem_value_h */