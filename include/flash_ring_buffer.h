#ifndef _flash_ring_buffer_h
#define _flash_ring_buffer_h

/**
 * @file	flash_ring_buffer.h
 * @author	en2
 * @date	24-04-2020
 * @brief	
 * @details	
**/

#include "flash_safe_value.h"

template<typename T>
class Flash_ring_buffer
{
    using Index = Flash_safe_value<int>;
    using Buffer = Flash_crc_value<T>;

    static constexpr auto size_item =  sizeof(Flash_crc_value<T>);
    static constexpr auto size_buffer = (number_sectors - 2) * size_sector;
    static constexpr auto items_per_buffer = size_buffer / size_item;
    static constexpr auto items_per_sector = size_sector / sizeof(Flash_crc_value<T>);

    struct Info {int address; int offset; Flash_sector & sector;};

    static_assert((size_sector % size_item) == 0);

public:
    Flash_ring_buffer();
    ~Flash_ring_buffer();

    Status init(Rfs_driver * driver);
    Status reset();

    Status push(T value);
    Result<T> pop();

    bool is_full();
    bool is_empty();

    int size_actual();
    int size_max();

protected:
    Info _head_info();
    Info _tail_info();

    Status _head_update();
    Status _size_update(bool direction);

private:
    Flash_sector _sector[size_sector];

    Index _head;
    Index _size;
}; /* class: Flash_ring_buffer */


template<typename T>
Flash_ring_buffer<T>::Flash_ring_buffer()
:
_head(0, _sector[0], _sector[1]),
_size(size_sector / 2, _sector[0], _sector[1])
{

}

template<typename T>
Flash_ring_buffer<T>::~Flash_ring_buffer()
{

}

template<typename T>
Status Flash_ring_buffer<T>::init(Rfs_driver * driver)
{
    for (int i = 0; i < number_sectors; i++)
    {
        _sector[i].number(i).driver(driver);
    }
    
    if (auto status_head = _head.load(); status_head == false) return status_head;
    if (auto status_size = _size.load(); status_size == false) return status_size;

    return true;
}

template<typename T>
Status Flash_ring_buffer<T>::reset()
{
    for (int i = 0; i < number_sectors; i++)
    {
        _sector[i].at(0).clear();
    }
        
    _head.set(_sector[2].address()).store();
    _size.set(0).store();

    return true;
}

template<typename T> 
Status Flash_ring_buffer<T>::push(T value)
{
    auto [head_address, head_offset, head_sector] = _head_info();

    if (head_address % size_sector == 0)
    {
        auto [status_empty, empty] = head_sector.is_empty(); 
        if (status_empty == false) return status_empty;

        if (empty == false)
        {
            if (auto status_clear = head_sector.at(0).clear(); status_clear == false) return status_clear;
            if (auto status_size = _size.set(_size.get() - items_per_sector).store(); status_size == false) return status_size;
        }
    }

    Buffer _buffer;

    if (auto status_buffer = _buffer.set(value).store_to(head_sector.at(head_offset)); status_buffer == false) return status_buffer;
    if (auto status_update_head = _head_update(); status_update_head == false) return status_update_head;
    if (auto status_update_size = _size_update(true); status_update_size == false) return status_update_size;
}

template<typename T> 
Result<T> Flash_ring_buffer<T>::pop()
{
    Result<T> result;

    if (is_empty()) return warning::value::Empty();

    auto [tail_address, tail_offset, tail_sector] = _tail_info();

    Buffer _buffer;

    if (auto status_buffer = _buffer.load_from(tail_sector.at(tail_offset)); status_buffer == false) return status_buffer;
    if (auto status_update_size = _size_update(false); status_update_size == false) return status_update_size;

    result.value = _buffer.get();

    return result;
}

template<typename T>
bool Flash_ring_buffer<T>::is_full()
{
    return (_size.get() == items_per_buffer);
}

template<typename T>
bool Flash_ring_buffer<T>::is_empty()
{
    return (_size.get() == 0);
}

template<typename T> 
int Flash_ring_buffer<T>::size_actual()
{
    return _size.get();
}

template<typename T>
int Flash_ring_buffer<T>::size_max()
{
    return items_per_buffer;
}

/* ---------------------------------------------| info |--------------------------------------------- */

template<typename T>
typename Flash_ring_buffer<T>::Info Flash_ring_buffer<T>::_head_info()
{
    auto address = _head.get();
    auto & sector = _sector[address / size_sector];
    auto offset = address - sector.address();

    return {(int) address, (int) offset,sector};
}

template<typename T>
typename Flash_ring_buffer<T>::Info Flash_ring_buffer<T>::_tail_info()
{
    auto address = _head.get() + size_buffer - size_actual() * size_item;
    auto & sector = _sector[address / size_sector];
    auto offset = address - sector.address();

    return {(int) address, (int) offset, sector};
}

template<typename T>
Status Flash_ring_buffer<T>::_head_update()
{
    _head.set((_head.get() + size_item));
    if (_head.get() == size_flash) _head.set(_sector[2].address());

    return _head.store();
}

template<typename T>
Status Flash_ring_buffer<T>::_size_update(bool direction)
{
    if (direction == true)
    {
        if (_size.get() < items_per_buffer) _size.set(_size.get() + 1);
    }
    else
    {
        if (_size.get() > 0) _size.set(_size.get() - 1);
    }

    return _size.store();
}


#endif /* define: flash_ring_buffer_h */