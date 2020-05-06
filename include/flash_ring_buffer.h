#ifndef _flash_ring_buffer_h
#define _flash_ring_buffer_h

/**
 * @file	flash_ring_buffer.h
 * @author	en2
 * @date	29-04-2020
 * @brief	
 * @details	
**/

#include "flash_value_safe.h"

template<typename T, CRC<T> crc> 
class Flash_ring_buffer
{
    static constexpr int size_item = sizeof(Flash_value_crc<T, crc>);
    static constexpr int size_buffer = (Flash_driver_generic::number_sector() - 2) * Flash_driver_generic::size_sector();
    static constexpr int number_items_buffer = size_buffer / size_item;
    static constexpr int number_items_sector = Flash_driver_generic::size_sector() / size_item;

    static constexpr int _bits = Flash_driver_generic::number_bits();

    using Buffer = Flash_value_crc<T, crc>;
    using Value = Flash_value_safe<int, crc, _bits>;

    struct Info {int address; int offset; Flash_sector & sector;};

    static_assert(Flash_driver_generic::size_sector() % sizeof(T) == 0, "Item size must fit into sector ...");

public:
    Flash_ring_buffer(Flash_driver_generic * driver);
    ~Flash_ring_buffer();

    Status init();
    Status reset();

    bool is_full();
    bool is_empty();
    int size_actual();
    int size_max();

    Status push(T value);
    Result<bool> pop();
    Result<T> at(int index);

protected:
    Info _head_info();
    Info _tail_info_at(int index = 0);

    Status _head_update();
    Status _size_update(int value);

private:
    Flash_sector _sector[Flash_driver_generic::number_sector()];

    Value _head;
    Value _size;

}; /* class: Flash_ring_buffer */

template<typename T, CRC<T> crc> 
Flash_ring_buffer<T, crc>::Flash_ring_buffer(Flash_driver_generic * driver)
:
_head(0, _sector[0], _sector[1]),
_size(Flash_driver_generic::size_sector() / 2, _sector[0], _sector[1])
{
    for (int i = 0; i < Flash_driver_generic::number_sector(); i++)
    {
        _sector[i].number(i);
        _sector[i].driver(driver);
    }   
}

template<typename T, CRC<T> crc> 
Flash_ring_buffer<T, crc>::~Flash_ring_buffer()
{

}

template<typename T, CRC<T> crc> 
Status Flash_ring_buffer<T, crc>::init()
{
    if (auto status_head = _head.load(); status_head == false) return status_head;
    if (auto status_size = _size.load(); status_size == false) return status_size;

    return true;
}

template<typename T, CRC<T> crc> 
Status Flash_ring_buffer<T, crc>::reset()
{
    for (int i = 0; i < Flash_driver_generic::number_sector(); i++)
    {
        if (auto status_clear = _sector[i].at(0).clear(); status_clear == false) return status_clear;
        _sector[i].at(0);
    }
        
    if (auto status_head = _head.set(_sector[2].address()).store(); status_head == false) return status_head;
    if (auto status_size = _size.set(0).store(); status_size == false) return status_size;

    return true;
}

template<typename T, CRC<T> crc> 
bool Flash_ring_buffer<T, crc>::is_full()
{
    return (_size.get() == number_items_buffer);
}

template<typename T, CRC<T> crc> 
bool Flash_ring_buffer<T, crc>::is_empty()
{
    return (_size.get() == 0);
}

template<typename T, CRC<T> crc> 
int Flash_ring_buffer<T, crc>::size_actual()
{
    return _size.get();
}

template<typename T, CRC<T> crc> 
int Flash_ring_buffer<T, crc>::size_max()
{
    return number_items_buffer;
}

/* ---------------------------------------------| main story |--------------------------------------------- */

template<typename T, CRC<T> crc> 
Status Flash_ring_buffer<T, crc>::push(T value)
{
    auto [head_address, head_offset, head_sector] = _head_info();

    if (head_address % Flash_driver_generic::size_sector() == 0)
    {
        auto [status_empty, empty] = head_sector.is_empty();
        if (status_empty == false) return status_empty;

        if (empty == false)
        {
            if (auto status_clear = head_sector.at(0).clear(); status_clear == false) return status_clear;

            if (_size.get() == number_items_buffer)
            {
                if (auto status_size = _size_update(-number_items_sector); status_size == false) return status_size;
            }
        }
    }

    Buffer _buffer;
    if (auto status_buffer = _buffer.set(value).store(head_sector.at(head_offset)); status_buffer == false) return status_buffer;
    
    if (auto status_update_head = _head_update(); status_update_head == false) return status_update_head;
    if (auto status_update_size = _size_update(+1); status_update_size == false) return status_update_size;

    return true;
}

template<typename T, CRC<T> crc> 
Result<bool> Flash_ring_buffer<T, crc>::pop()
{
    Result<bool> result;

    if (is_empty()) return status::warning::possible::Value_empty();

    if (auto status_update_size = _size_update(-1); status_update_size == false) return status_update_size;

    result.value = true;

    return result;
}

template<typename T, CRC<T> crc>
Result<T> Flash_ring_buffer<T, crc>::at(int index)
{
    Result<T> result;

    if (is_empty()) return status::warning::possible::Value_empty();
    if (index > size_actual()) return status::error::argument::Out_of_range();

    auto [tail_address, tail_offset, tail_sector] = _tail_info_at(index);

    Buffer _buffer;
    if (auto status_buffer = _buffer.load(tail_sector.at(tail_offset)); status_buffer == false) return status_buffer;

    result.value = _buffer.get();

    return result;
}

/* ---------------------------------------------| info |--------------------------------------------- */

template<typename T, CRC<T> crc> 
typename Flash_ring_buffer<T, crc>::Info Flash_ring_buffer<T, crc>::_head_info()
{
    auto address = _head.get();
    auto & sector = _sector[address / Flash_driver_generic::size_sector()];
    auto offset = address - sector.address();

    return {(int) address, (int) offset,sector};
}
template<typename T, CRC<T> crc> 
typename Flash_ring_buffer<T, crc>::Info Flash_ring_buffer<T, crc>::_tail_info_at(int index)
{
    auto size_desired = _size.get() - index;

    auto address = _head.get()  - size_desired * size_item < _sector[2].address() ? _head.get() + size_buffer - size_desired * size_item : _head.get() - size_desired * size_item ;
    auto & sector = _sector[address / Flash_driver_generic::size_sector()];
    auto offset = address - sector.address();

    return {(int) address, (int) offset, sector};
}
template<typename T, CRC<T> crc> 
Status Flash_ring_buffer<T, crc>::_head_update()
{
    _head.set((_head.get() + size_item));

    if (_head.get() == Flash_driver_generic::size_flash()) _head.set(_sector[2].address());

    return _head.store();
}
template<typename T, CRC<T> crc> 
Status Flash_ring_buffer<T, crc>::_size_update(int value)
{
    if (((_size.get() + value) >= 0) && ((_size.get() + value) <= number_items_buffer)) _size.set(_size.get() + value);

    return _size.store();
}

#endif /* define: flash_ring_buffer_h */