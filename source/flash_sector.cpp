#include "flash_sector.h"

Flash_sector::Flash_sector(int number, Flash_driver * driver)
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

bool Flash_sector::reset()
{
    return _driver->erase(_number * size);
}

int Flash_sector::clear(int value)
{
    // if (size < 1) return -1;

    // char buffer[size];
    // auto stored = _at;

    // if (at(0).read_to(buffer) != size) return -1;
    // if (reset() == false) return -1;

    // auto space = (stored + size >= size) ? size - stored : size;

    // memset(&buffer[stored], 0xff, space);

    // return at(0).write_from(buffer) == size ? space : -1;
}