#include "test.h"
#include "flash_sector.h"
#include "flash_driver_file.h"

TEST_CASE("sector is empty")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    REQUIRE(sector.is_empty().value == true);

    unsigned char temp = 0x12;
    driver.write(0, 1, &temp);

    REQUIRE(sector.is_empty().value == false);

    driver.deinit();
}

TEST_CASE("sector is full")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    REQUIRE(sector.is_full().value == false);

    unsigned char temp = 0x12;
    driver.write(driver.size_sector() - 1, 1, &temp);

    REQUIRE(sector.is_full().value == true);

    driver.deinit();
}

TEST_CASE("sector clear")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    unsigned char compare[512];
    
    for (int i = 0; i < sizeof(compare); i++)
    {
        compare[i] = 0xff;

        unsigned char trash = i ^ 0x55;

        driver.write(i, 1, &trash);
    }
    
    REQUIRE(sector.clear() == true);

    unsigned char rx[512];

    driver.read(0, sizeof(rx), rx);

    REQUIRE(memcmp(compare, rx, 512) == 0);

    driver.deinit();  
}

TEST_CASE("sector write driver read")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    int tx = 0xdeadbeef;
    int rx;

    sector.write(tx);
    driver.read(0, sizeof(rx), (unsigned char *)&rx);

    REQUIRE(rx == tx);

    driver.deinit();
}

TEST_CASE("sector read driver write")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    int tx = 0xdeadbeef;

    driver.write(0, sizeof(tx), (unsigned char *)&tx);
    auto [status, rx] = sector.read<int>();

    REQUIRE(rx == tx);

    driver.deinit();
}

TEST_CASE("sector at moving write")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    int tx;

    for (int i = 0; i < driver.size_sector() / sizeof(int); i++)
    {
        sector.write(tx);
        REQUIRE(sector.at() == (i + 1) * sizeof(tx));
    }
    
    driver.deinit();
}

TEST_CASE("sector at moving read")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    for (int i = 0; i < driver.size_sector() / sizeof(int); i++)
    {
        sector.read<int>();
        REQUIRE(sector.at() == (i + 1) * sizeof(int));
    }
    
    driver.deinit();
}

TEST_CASE("sector write read")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    int tx = 0xdeadbeef;

    for (int i = 0; i < driver.size_sector() / sizeof(int); i++)
    {
        sector.write(tx);

        auto result = sector.at(sector.at() - sizeof(int)).read<int>();

        REQUIRE(result.value == tx);
    }
    
    driver.deinit();
}

TEST_CASE("sector write partial")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);

    driver.init();

    int tx = 0xdeadbeef;

    sector.at(0).write_partial(tx, 1, 3);

    auto result = sector.at(0).read<int>();

    REQUIRE(result.value == 0xdeadbeff);

    driver.deinit();  
}