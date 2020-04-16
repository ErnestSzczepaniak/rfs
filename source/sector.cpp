#include "sector.h"
#include "string.h"
#include <stdint.h>

Sector::Sector(int number, Flash_driver * driver)
:
_number(number),
_driver(driver),
_cell(0)
{

}

Sector::~Sector()
{

}

Sector & Sector::number(int number)
{
    _number = number;

    return *this;
}

int Sector::number()
{
    return _number;
}

Sector & Sector::cell(int value)
{
    _cell = value;

    return *this;
}

int Sector::cell()
{
    return _cell;
}

bool Sector::erase()
{
    return  _driver->erase(_number * _driver->size_sector);
}

bool Sector::clear(int records, bool verify)
{
    auto to_clear = (_cell + records >= _driver->size_sector / size_base) ? (_driver->size_sector / size_base) - _cell : records;
    auto stored = _cell;
    unsigned int buffer_tx[_driver->size_sector / size_base];

    if (cell(0).read(buffer_tx) == false) return false;

    memset(&buffer_tx[stored], 0xff, to_clear * size_base);

    if (erase() == false) return false;

    if (cell(0).write(buffer_tx) == false) return false;

    if (verify)
    {
        unsigned int buffer_rx[_driver->size_sector / size_base];

        if (cell(0).read(buffer_rx) == false) return false;

        for (int i = 0; i < _driver->size_sector / size_base; i++)
        {
            if (buffer_tx[i] != buffer_rx[i]) return false;
        }   
    }


    _cell = stored + to_clear;

    return true;
}

/* ---------------------------------------------| info |--------------------------------------------- */

unsigned int Sector::_crc_calculate(unsigned int * data, int size)
{
    unsigned int crc = 0;
    unsigned char * ptr = (unsigned char *) data;

	for (int i = 0; i < size; i++)
	{
		auto pos = (crc ^ (ptr[i] << 24)) >> 24;
		crc = (crc << 8) ^ (crc_table[pos]);
	}

	return crc;
}