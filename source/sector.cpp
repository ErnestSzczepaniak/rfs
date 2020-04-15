#include "sector.h"
#include "string.h"

Sector::Sector(int number, Flash_driver * driver)
:
_address(number * driver->size_sector),
_driver(driver)
{

}

Sector::~Sector()
{

}

bool Sector::errase()
{

}

bool Sector::read(int offset, int size, unsigned char * to)
{

}

bool Sector::write(int offset, int size, unsigned char * from)
{
    unsigned int buffer[size_buffer];

    // memcpy(buffer, from, size);

    // return _driver->write(offset, size, &buffer);
}

