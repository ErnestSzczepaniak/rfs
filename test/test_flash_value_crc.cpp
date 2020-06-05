#include "test.h"
#include "flash_value_crc.h"
#include "flash_driver_file.h"
#include "crc.h"

TEST_CASE("value crc store driver read")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_crc<int, crc<32>> value;

    driver.init();

    REQUIRE(value.set(0xdeadbeef).store(sector) == true);

    int rx;
    unsigned int crc;

    driver.read(0, sizeof(int), (unsigned char *) &rx);
    driver.read(sizeof(unsigned int), sizeof(unsigned int), (unsigned char *) &crc);

    REQUIRE(rx == 0xdeadbeef);
    REQUIRE(crc == 0x2AE4BF98);

    driver.deinit();
}

TEST_CASE("value crc load driver write")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_crc<int, crc<32>> value;

    driver.init();

    int tx = 0xdeadbeef;
    unsigned int crc = 0x2AE4BF98;

    driver.write(0, sizeof(int), (unsigned char *) &tx);
    driver.write(sizeof(int), sizeof(unsigned int), (unsigned char *) &crc);

    auto result = value.load(sector);
    REQUIRE(result == status::binary::Success());

    REQUIRE(value.get() == tx);

    driver.deinit();
}

TEST_CASE("value crc store load")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_crc<int, crc<32>> value;

    driver.init();

    REQUIRE(value.set(0xdeadbeef).store(sector) == true);

    sector.at(sector.at() - sizeof(Flash_value_crc<int, crc<32>>));

    auto result = value.load(sector);
    REQUIRE(result == status::binary::Success());

    REQUIRE(value.get() == 0xdeadbeef);

    driver.deinit();
}

TEST_CASE("value crc mismatch")
{
    Flash_driver_file driver;
    Flash_sector sector(0, &driver);
    Flash_value_crc<int, crc<32>> value;

    driver.init();

    REQUIRE(value.set(0xdeadbeef).store(sector) == true);

    unsigned char trash = 0x21;
    driver.write(sizeof(int) + 1, 1, &trash);

    sector.at(sector.at() - sizeof(Flash_value_crc<int, crc<32>>));

    auto result = value.load(sector);
    REQUIRE(result == status::error::frame::Crc_mismatch());

    driver.deinit(); 
}