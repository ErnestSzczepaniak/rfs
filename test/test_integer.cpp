#include "test.h"
#include "flash_integer.h"
#include "flash_driver_software.h"

TEST_CASE("flash variable")
{
    _init();

    Flash_sector s(0, &driver);
    //s.reset();

    Flash_integer in(s);
    Flash_integer i2(s);

    i2.set(0);
    i2.restore();

    REQUIRE(i2.get() == 0);

    for (int i = 0; i < 145; i++)
    {
        auto res = in.set(i);
        printf("s = %s\n", res.category());

        in.restore();
        REQUIRE(in.get() == i);
    }
    


}