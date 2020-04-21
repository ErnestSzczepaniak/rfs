#include "flash_sector.h"

Flash_sector::Flash_sector(int number, Rfs_driver * driver)
:
_number(number),
_driver(driver),
_at(0)
{

}

Flash_sector::~Flash_sector()
{

}

Flash_sector & Flash_sector::number(int number)
{
    _number = number;

    return *this;
}

int Flash_sector::number()
{
    return _number;
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

Status Flash_sector::reset()
{
    return _driver->erase(_number * size_sector);
}

Status Flash_sector::clear(int value)
{
    Sector sector;

    if (value < 1) return error::argument::Out_of_range();
    if (_at + value > size_sector) return error::argument::Out_of_range();
    if (_driver->read(_number * size_sector, size_sector, (char *)&sector.payload[0]) == false) return error::driver::Read();
    if (auto status_reset = reset(); status_reset == false) return status_reset;

    memset(&sector.payload[_at], 0xff, value);

    return write(sector);
}

Result<Sector> Flash_sector::read()
{
    return at(0).read_value<Sector>();
}

Status Flash_sector::write(Sector & sector)
{
    return at(0).write_value(sector);
}

Result<bool> Flash_sector::verify_with(Flash_sector & other)
{
    auto [status_other, sector_other] = other.read();
    if (status_other == false) return status_other;

    return at(0).verify_with_value(sector_other);
}

Status Flash_sector::replace_with(Flash_sector & other)
{
    auto [status_other, sector_other] = other.read();
    if (status_other == false) return status_other;

    return at(0).replace_with_value(sector_other); 
}