#include "test.h"
#include "flash_filesystem_value.h"
#include "flash_driver_software.h"

TEST_CASE("flash variable")
{
    _init();

    Flash_sector sector;

    sector.number(0).driver(&driver).at(0).clear();

    Flash_filesystem_value<int> flash_value;

    flash_value.load_from(sector.at(0));

    flash_value.set(20).set(50).store_to(sector.at(0));

    flash_value.load_from(sector.at(0));

    REQUIRE(flash_value.get() == 50);

    // Flash_filesystem_value<int> value;

    // for (int i = 0; i < 1000; i++)
    // {
    //     value.update(0, s, i);
    //     auto res = value.restore(0, s);

    //     REQUIRE(value.value() == i);
    //     REQUIRE(res.operator bool() == true);
    // }
}