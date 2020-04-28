#include "test.h"
#include "flash_value_filesystem.h"
#include "flash_driver_file.h"

TEST_CASE("value filesystem usage")
{
    Flash_driver_file driver;
    Flash_sector sector(0, driver);
    Flash_value_filesystem<int, crc<32>, 8> value;

    driver.init();

    unsigned char temp = 0xff;

    for (int i = 0; i < 8; i++)
    {
        temp &= ~(0x80 >> i);

        driver.write(0, 1, &temp);

        value.load(sector.at(0));

        REQUIRE(value.usage() == i + 1);
        REQUIRE(value.get() == 0xffffffff);
    }
    
    driver.deinit();
}