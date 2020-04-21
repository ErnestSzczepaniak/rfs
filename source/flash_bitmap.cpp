#include "flash_bitmap.h"

Flash_bitmap::Flash_bitmap(int address, Flash_sector & sector)
:
_address(address),
_sector(sector)
{

}

Flash_bitmap::~Flash_bitmap()
{

}

Flash_bitmap & Flash_bitmap::address(int value)
{
    _address = value;

    return *this;
}

int Flash_bitmap::address()
{
    return _address;
}

Flash_bitmap & Flash_bitmap::sector(Flash_sector & value)
{
    _sector = value;

    return *this;
}

Flash_sector & Flash_bitmap::sector()
{
    return _sector;
}

Status Flash_bitmap::reset()
{
    return _sector.at(_address).clear(size_bitmap);
}

Status Flash_bitmap::clear(int bit)
{
    if (bit > number_bits || bit < 0) return error::argument::Out_of_range();

    auto offset = bit / 8;

    auto [status_read, byte] = _sector.at(_address + offset).read_value<unsigned char>();
    if (status_read == false) return status_read;

    auto mask = (0x80 >> (bit % 8));

    if ((byte & mask) == 0) return true; // bit already set

    byte &= ~mask;

    if (auto status_write = _sector.at(_address + offset).write_value(byte); status_write == false) return status_write;

    auto [status_verify, success] = _sector.at(_address + offset).verify_with_value(byte);

    if (status_verify == false) return status_verify;

    return (success == true);
}

Result<bool> Flash_bitmap::is_cleared(int bit)
{
    Result<bool> result;

    if (bit > number_bits || bit < 0) return error::argument::Out_of_range();

    auto offset = bit / 8;

    auto [status_read, byte] = _sector.at(_address + offset).read_value<unsigned char>();
    if (status_read == false) return status_read;

    auto mask = (0x80 >> (bit % 8));

    result.value = ((byte & mask) == 0);

    return result;
}

Result<int> Flash_bitmap::count_cleared()
{
    Result<int> result;
    result.value = 0;

    for (int i = 0; i < size_bitmap; i++)
    {
        auto [status_read, byte] = _sector.at(_address + i).read_value<unsigned char>();
        if (status_read == false) return status_read;

        for (int j = 0; j < 8; j++)
        {
            if ((byte & (0x80 >> j)) == 0) result.value++;
            else return result;
        }
    }

    return result;
}
