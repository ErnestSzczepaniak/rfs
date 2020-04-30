#include "test.h"
#include "flash_value_filesystem.h"
#include "flash_driver_file.h"
#include "crc.h"

TEST_CASE("value filesystem usage")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_filesystem<int, crc<32>> value;

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

TEST_CASE("value filesystem write")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_filesystem<int, crc<32>, 8> value;

    driver.init();

    unsigned char compare_map = 0xff;
    unsigned char real_map;

    int compare_item;
    int real_item;

    for (int i = 0; i < 21; i++)
    {
        value.set(i).store(sector.at(0));

        driver.read(0, 1, (unsigned char *) &real_map);
        driver.read(1 + (i % 8) * sizeof(Flash_value_crc<int, crc<32>>), sizeof(int), (unsigned char *) &real_item);

        compare_map &= ~(0x80 >> (i % 8));

        REQUIRE(real_map == compare_map);
        REQUIRE(real_item == i);

        if (i % 8 == 7) compare_map = 0xff;
    }
    
    driver.deinit();
}

TEST_CASE("value filesystem read")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_filesystem<int, crc<32>, 8> value;

    driver.init();

    unsigned char compare_map = 0xff;

    for (int i = 0; i < 18; i++)
    {
        compare_map &= ~(0x80 >> (i % 8));

        driver.write(0, 1, (unsigned char *) &compare_map);

        int compare_item = i;
        unsigned int compare_crc = crc<32>(compare_item);

        driver.write(1 + (i % 8) * sizeof(Flash_value_crc<int, crc<32>>), sizeof(int), (unsigned char *) &compare_item);
        driver.write(1 + (i % 8) * sizeof(Flash_value_crc<int, crc<32>>) + sizeof(int), sizeof(int), (unsigned char *) &compare_crc);

        value.load(sector.at(0));

        REQUIRE(value.get() == i);
        REQUIRE(value.usage() == i % 8 + 1);

        if (i % 8 == 7) compare_map = 0xff;
    }
    
    driver.deinit();
}

TEST_CASE("value filesystem write read")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_filesystem<int, crc<32>, 16> value;

    driver.init();

    for (int i = 0; i < 17; i++)
    {
        value.set(i).store(sector.at(0));

        value.load(sector.at(0));

        REQUIRE(value.get() == i);
    }

    driver.deinit();
}

TEST_CASE("value filesystem recover")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_filesystem<int, crc<32>, 16> value;

    driver.init();

    value.set(1).store(sector.at(0));
    value.set(2).store(sector.at(0));

    REQUIRE(value.usage() == 2);

    unsigned char trash = 0x12;

    sector.at(14).write(trash);

    auto status = value.load(sector.at(0));
    REQUIRE(status == status::error::frame::Crc_mismatch());

    REQUIRE(value.get() == 2);

    value.recover(sector.at(0));

    REQUIRE(value.get() == 1);
    REQUIRE(value.usage() == 3);

    driver.deinit(); 
}