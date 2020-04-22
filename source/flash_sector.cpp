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

int Flash_sector::at()
{
    return _at;
}

Status Flash_sector::clear(int range)
{
    return _driver->clear(_at, range);
}