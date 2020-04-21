#include "flash_integer.h"

int Flash_integer::_address_pointer = 0;

Flash_integer::Flash_integer(Flash_sector & sector)
:
_bitmap(_address_pointer, sector)
{
    _address_pointer += size_bitmap + (number_bits * sizeof(unsigned int));
}

Flash_integer::~Flash_integer()
{

}

Status Flash_integer::restore()
{
    auto [result_count, count] = _bitmap.count_cleared();
    if (result_count == false) return result_count;

    auto offset = size_bitmap + (count -1) * sizeof(unsigned int);

    auto  [result_read, value] = _bitmap.sector().at(_bitmap.address() + offset).read_value<unsigned int>();
    if (result_read == false) return result_read;

    _value = value;

    return true;
}

unsigned int Flash_integer::get()
{
    return _value;
}

Status Flash_integer::set(unsigned int value)
{
    auto [result_count, count] = _bitmap.count_cleared();
    if (result_count == false) return result_count;

    if (count < number_bits) // have space
    {
        auto offset = size_bitmap + count * sizeof(unsigned int);

        if (auto status_write = _bitmap.sector().at(_bitmap.address() + offset).write_value(value); status_write == false) return status_write;
        if (auto status_update = _bitmap.clear(count); status_update == false) return status_update;

        _value = value;

        return true;
    }
    else // no space, reset needed
    {
        if (auto result_clear = _bitmap.sector().at(_bitmap.address()).clear(size_bitmap + (number_bits * sizeof(unsigned int))); result_clear == false) return result_clear;
        if (auto status_write = _bitmap.sector().at(_bitmap.address() + size_bitmap).write_value(value); status_write == false) return status_write;
        if (auto status_update = _bitmap.clear(0); status_update == false) return status_update;

        _value = value;

        return true;
    }
}
