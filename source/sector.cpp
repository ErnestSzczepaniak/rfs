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
    return _driver->erase(_address);
}

bool Sector::read(int offset, int size, unsigned char * to)
{
    auto offset_reminder = offset % 4;
    auto size_reminder = offset_reminder + 4 - (size + offset_reminder) % 4;

    unsigned int buffer[size_buffer];
    auto * ptr = (unsigned char *)buffer;

    _driver->read(offset - offset_reminder, size + size_reminder, buffer);

    memcpy(to, ptr + offset_reminder, size);
}

bool Sector::write(int offset, int size, unsigned char * from)
{
    auto offset_reminder = offset % 4;
    auto size_reminder = offset_reminder + 4 - (size + offset_reminder) % 4;

    unsigned int buffer[size_buffer];
    auto * ptr = (unsigned char *)buffer;

    memcpy(ptr + offset_reminder, from, size);

    return _driver->write(offset - offset_reminder, size + size_reminder, buffer);
}

