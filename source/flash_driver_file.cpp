#ifdef build_platform_host

#include "flash_driver_file.h"

bool Flash_driver_file::init()
{
    _stream.open("/home/en2/Documents/programs/rfs/flash_file", std::ios::out | std::ios::in);

    if (_stream.is_open() == true) return true;

    _stream.open("/home/en2/Documents/programs/rfs/flash_file", std::ios::out | std::ios::in | std::ios::trunc);

    if (_stream.is_open() == false) return false;

    char value = 0xff;

    for (int i = 0; i < size_flash(); i++)
    {
        _stream.write((char *) &value, 1);
    }

    _stream.flush();

    return true;
}

bool Flash_driver_file::deinit()
{
    remove("/home/en2/Documents/programs/rfs/flash_file");

    return true;
}

bool Flash_driver_file::read(int address, int size, unsigned char * data)
{
    _stream.seekg(address, std::ios::beg);
    _stream.read((char *) data, size);
    _stream.flush();

    return true;
}

bool Flash_driver_file::write(int address, int size, unsigned char * data)
{
    _stream.seekp(address, std::ios::beg);
    _stream.write((char *) data, size);
    _stream.flush();

    return true;
}

bool Flash_driver_file::erase(int address, int span)
{
    unsigned char value = 0xff;

    for (int i = 0; i < span; i++)
    {
        write(address + i, 1, &value);
    }

    return true;
}

#endif