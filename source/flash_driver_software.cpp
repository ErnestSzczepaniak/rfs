#include "flash_driver_software.h"
#include <fstream>

std::fstream stream;

static constexpr auto _size_sector = 64;
static constexpr auto _size_total = 128;

bool _init()
{
    stream.open("/home/en2/Documents/programs/rfs/flash", std::ios::out | std::ios::in);

    if (stream.is_open() == true) return true;

    stream.open("/home/en2/Documents/programs/rfs/flash", std::ios::out | std::ios::in | std::ios::trunc);

    if (stream.is_open() == false) return false;

    char value = 0xff;

    for (int i = 0; i < _size_total; i++)
    {
        stream.write((char *) &value, 1);
    }

    return true;
}

/* ---------------------------------------------| driver |--------------------------------------------- */

bool _read(int address, int size, char * buffer)
{
    stream.seekg(address, std::ios::beg);
    stream.read((char *) buffer, size);

    return true;
}

bool _write(int address, int size, char * buffer)
{
    stream.seekp(address, std::ios::beg);
    stream.write((char *) buffer, size);

    return true;
}

bool _erase(int address)
{
    auto offset = (address / _size_sector) * _size_sector;

    char value = 0xff;

    for (int i = 0; i < _size_sector; i++)
    {
        _write(i + offset, 1, &value);
    }

    return true;
}

Flash_driver driver = {_read, _write, _erase};