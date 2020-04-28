#include "test.h"
#include "flash_value.h"
#include "flash_driver_file.h"

TEST_CASE("value store driver read")
{
    Flash_driver_file driver;
    Flash_sector sector(0, driver);
    Flash_value<int> value;

    driver.init();

    value.set(0xdeadbeef);
    value.store(sector);

    int rx;
    driver.read(0, sizeof(int), (unsigned char *) &rx);

    REQUIRE(value.get() == rx);

    driver.deinit();
}

TEST_CASE("value load driver write")
{
    Flash_driver_file driver;
    Flash_sector sector(0, driver);
    Flash_value<int> value;

    driver.init();

    int tx = 0xdeadbeef;
    driver.write(0, sizeof(int), (unsigned char *) &tx);

    value.load(sector);

    REQUIRE(value.get() == tx);

    driver.deinit();
}

TEST_CASE("value store load")
{
    Flash_driver_file driver;
    Flash_sector sector(0, driver);
    Flash_value<int> value;

    driver.init();

    value.set(0xdeadbeef);

    value.store(sector);

    sector.at(sector.at() - sizeof(int));

    value.load(sector);

    REQUIRE(value.get() == 0xdeadbeef);

    driver.deinit();   
}