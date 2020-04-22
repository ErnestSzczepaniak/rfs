#include "flash_sector.h"

Flash_sector::Flash_sector()
{

}

Flash_sector::~Flash_sector()
{

}

Flash_sector & Flash_sector::bind(int number, Rfs_driver * driver)
{
    _number = number;
    _driver = driver;

    return *this;
}

Status Flash_sector::clear(int address, int range)
{
    return _driver->clear(address, range);
}