#include "flash_sector.h"

Flash_sector::Flash_sector(int number, Flash_driver_generic & driver)
:
_at(0),
_number(number),
_driver(driver)
{

}

Flash_sector::~Flash_sector()
{

}

Flash_sector & Flash_sector::number(int number)
{
    if (number < _driver.number_sector()) _number = number;

    return *this;
}

int Flash_sector::number()
{
    return _number;
}

Flash_sector & Flash_sector::address(int address)
{
    if (address < _driver.size_flash() && (address % _driver.size_sector() == 0)) _number = address / _driver.size_sector();

    return *this;
}

int Flash_sector::address()
{
    return _number * _driver.size_sector();
}

Flash_sector & Flash_sector::driver(Flash_driver_generic & driver)
{
    _driver = driver;

    return *this;
}

Flash_driver_generic & Flash_sector::driver()
{   
    return _driver;
}

Flash_sector & Flash_sector::at(int value)
{
    _at = value;

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

Result<bool> Flash_sector::is_full()
{
    Result<bool> result;

    auto [status, byte] = at(_driver.size_sector() - 1).read<unsigned char>();
    if (status == false) return status;

    result.value = (byte != 0xff);

    return result;
}

Status Flash_sector::clear(int range)
{
    if (range == 0) range = _driver.size_sector(); 
    if (_at + range > _driver.size_sector()) return error::argument::Out_of_range();
    if (auto status_clear = _driver.erase(_number * _driver.size_sector() + _at, range); status_clear == false) return status_clear;

    _at += range;

    return true;
}