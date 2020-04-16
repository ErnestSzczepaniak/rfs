#include "flash_driver_software.h"
#include <fstream>

std::fstream stream;

bool _init()
{
    stream.open("/home/en2/Documents/programs/rfs/flash", std::ios::out | std::ios::in);

    if (stream.is_open() == true) return true;

    stream.open("/home/en2/Documents/programs/rfs/flash", std::ios::out | std::ios::in | std::ios::trunc);

    if (stream.is_open() == false) return false;

    char value = 0xff;

    for (int i = 0; i < driver.size_total; i++)
    {
        stream.write((char *) &value, 1);
    }

    return true;
}

/* ---------------------------------------------| driver |--------------------------------------------- */

bool _read(unsigned int address, int size, unsigned int * buffer)
{
    if (address & 0x3) return false;
    if (size & 0x3) return false;
    if (((uintptr_t) buffer) & 0x3) return false;

    stream.seekg(address, std::ios::beg);
    stream.read((char *) buffer, size);

    return true;
}

bool _write(unsigned int address, int size, unsigned int * buffer)
{
    if (address & 0x3) return false;
    if (size & 0x3) return false;
    if (((uintptr_t) buffer) & 0x3) return false;

    stream.seekp(address, std::ios::beg);
    stream.write((char *) buffer, size);

    return true;
}

bool _erase(unsigned int address)
{
    auto offset = (address / driver.size_sector) * driver.size_sector;

    unsigned int value = 0xffffffff;

    for (int i = 0; i < driver.size_sector / 4; i++)
    {
        _write(i * 4 + offset, 4, &value);
    }
}

Flash_driver driver = {_read, _write, _erase};