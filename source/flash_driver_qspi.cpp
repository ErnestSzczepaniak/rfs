#ifdef build_platform_target

#include "flash_driver_qspi.h"
#include "hal.h"
#include "string.h"

unsigned int Flash_driver_qspi::_buffer[] = {};

bool Flash_driver_qspi::init()
{
    return h_qspi_init();
}

bool Flash_driver_qspi::deinit()
{
    return h_qspi_deinit();
}

bool Flash_driver_qspi::read(int address, int size, unsigned char * data)
{
    auto start_reminder = address % 4;
    auto sub_reminder = (size + start_reminder) % 4;
    auto size_reminder = sub_reminder ? 4 - sub_reminder : 0;

    auto * ptr = (unsigned char *) _buffer;

    if (auto result = h_qspi_read(address - start_reminder, size + start_reminder + size_reminder, ptr); result == false) return result;

    memcpy(data, ptr + start_reminder, size);

    return true;
}

bool Flash_driver_qspi::write(int address, int size, unsigned char * data)
{
    auto start_reminder = address % 4;
    auto sub_reminder = (size + start_reminder) % 4;
    auto size_reminder = sub_reminder ? 4 - sub_reminder : 0;

    auto * ptr = (unsigned char *) _buffer;

    for (int i = 0; i < start_reminder; i++)
    {
        ptr[i] = 0xff;
    }
    
    for (int i = 0; i < size_reminder; i++)
    {
        ptr[start_reminder + size + i] = 0xff;
    }

    memcpy(ptr + start_reminder, data, size);

    return _write_partial(address - start_reminder, size + start_reminder + size_reminder, (unsigned char *) _buffer);
}

bool Flash_driver_qspi::erase(int address, int size) 
{
    auto size_sector = Flash_driver_generic::size_sector();

    if (size == size_sector) return h_qspi_erase(address);
    
    unsigned char buffer[size_sector];

    auto base_address = (address / size_sector) * size_sector;

    if (auto status_read = read(base_address, size_sector, buffer); status_read == false) return false;

    auto first_size = address - base_address;
    auto second_size = size_sector - first_size - size;

    if (auto status_erase = h_qspi_erase(address); status_erase == false) return status_erase;
    if (auto status_write_first = write(base_address, first_size, buffer); status_write_first == false) return status_write_first;
    if (auto status_write_second = write(address + size, second_size, &buffer[first_size + size]); status_write_second == false) return status_write_second;

    return true;
}

bool Flash_driver_qspi::_write_partial(int address, int size, unsigned char * data)
{
    auto full_beats = size / 256;
    auto last_beat = size % 256;

    for (int i = 0; i < full_beats; i++)
    {
        if (auto result = h_qspi_write(address + i * 256, 256, &data[i * 256]); result == false) return result;
    }
    
    return h_qspi_write(address + full_beats * 256, last_beat, &data[full_beats * 256]);
}

#endif