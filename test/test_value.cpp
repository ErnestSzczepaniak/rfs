#include "test.h"
#include "flash_filesystem_value.h"
#include "flash_driver_software.h"

struct Log
{
    unsigned char id;
    unsigned char value;
    unsigned char timestamp[6];
}; /* structure: Log */

static_assert(sizeof(Log) == 8);

TEST_CASE("flash variable")
{
    // _init();

    // Flash_sector sector;

    // sector.number(0).driver(&driver).at(0).clear();

    // Flash_filesystem_value<int> flash_value;

    // for (int i = 0; i < 100; i++)
    // {
    //     flash_value.set(i).store_to(sector.at(0));

    //     flash_value.load_from(sector.at(0));

    //     REQUIRE(flash_value.get() == i);
    // }
}