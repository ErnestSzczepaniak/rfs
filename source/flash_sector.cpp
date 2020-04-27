#include "flash_sector.h"

Flash_sector & Flash_sector::number(int value)
{
    _number = value;

    return *this;
}

int Flash_sector::number()
{
    return _number;
}

int Flash_sector::address()
{
    return _number * size_sector;
}

Flash_sector & Flash_sector::driver(Rfs_driver * driver)
{
    _driver = driver;

    return *this;
}

Rfs_driver * Flash_sector::driver()
{   
    return _driver;
}

Flash_sector & Flash_sector::at(int value)
{
    _at = value;

    return *this;
}

Flash_sector & Flash_sector::at_offset(int value)
{
    _at += value;

    return *this;
}

int Flash_sector::at()
{
    return _at;
}

Result<bool> Flash_sector::is_empty()
{
    Result<bool> result;

    auto [status, byte] = at(0).read<unsigned char>();
    if (status == false) return status;

    result.value = (byte == 0xff);

    return result;
}

Status Flash_sector::clear(int range)
{
    if (_at + range > size_sector) return error::argument::Out_of_range();
    if (auto status_clear = _driver->clear(_number * size_sector + _at, range); status_clear == false) return status_clear;

    _at += range;

    return true;
}