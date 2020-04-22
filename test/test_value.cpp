#include "test.h"
#include "flash_filesystem_value.h"
#include "flash_driver_software.h"

TEST_CASE("flash variable")
{
    _init();

    Flash_sector s;

    s.bind(0, &driver);
    s.clear();


    Flash_value<int> f;

    // Flash_filesystem_value<int> value;

    // for (int i = 0; i < 1000; i++)
    // {
    //     value.update(0, s, i);
    //     auto res = value.restore(0, s);

    //     REQUIRE(value.value() == i);
    //     REQUIRE(res.operator bool() == true);
    // }
}