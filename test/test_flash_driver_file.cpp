#include "flash_driver_file.h"
#include "test.h"
#include "string.h"

TEST_CASE("driver file init deinit")
{
    Flash_driver_file driver;

    REQUIRE(driver.init() == true);
    REQUIRE(driver.deinit() == true);
}

TEST_CASE("driver file write read")
{
    Flash_driver_file driver;

    unsigned char tx[2048];
    unsigned char rx[2048];

    for (int i = 0; i < sizeof(tx); i++)
    {
        tx[i] = i ^ 0x34;
    }
    
    REQUIRE(driver.init() == true);

    REQUIRE(driver.write(0, sizeof(tx), tx) == true);
    REQUIRE(driver.read(0, sizeof(rx), rx) == true);
   
    REQUIRE(memcmp(tx, rx, sizeof(tx)) == 0);

    REQUIRE(driver.deinit() == true);
}

TEST_CASE("driver file write erase read")
{
    Flash_driver_file driver;

    constexpr auto size = 128;

    unsigned char tx[size];
    unsigned char rx[size];
    unsigned char efs[size / 2];

    for (int i = 0; i < size; i++)
    {
        tx[i] = i ^ 0x34;
        if (i < size / 2) efs[i] = 0xff;
    }
    
    REQUIRE(driver.init() == true);

    REQUIRE(driver.write(0, size, tx) == true);
    REQUIRE(driver.erase(size / 2, size / 2) == true);
    REQUIRE(driver.read(0, size, rx) == true);

    REQUIRE(memcmp(tx, rx, size / 2) == 0);
    REQUIRE(memcmp(efs, &rx[size / 2], size / 2) == 0);

    REQUIRE(driver.erase(0, 1) == true);
    REQUIRE(driver.read(0, size, rx) == true);
    REQUIRE(rx[0] == 0xff);

    REQUIRE(driver.deinit() == true);
}