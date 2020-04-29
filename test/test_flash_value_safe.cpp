#include "test.h"
#include "flash_value_safe.h"
#include "flash_driver_file.h"
#include "crc.h"

TEST_CASE("value safe write")
{
    Flash_driver_file driver;
    Flash_sector primary(0, &driver);
    Flash_sector secondary(1, &driver);

    Flash_value_safe<int, crc<32>, 8> value(0, primary, secondary);

    driver.init();

    unsigned char compare_map = 0xff;
    unsigned char real_map_primary;
    unsigned char real_map_secondary;

    int compare_item;
    int real_item_primary;
    int real_item_secondary;

    for (int i = 0; i < 21; i++)
    {
        value.set(i).store();

        driver.read(0, 1, (unsigned char *) &real_map_primary);
        driver.read(Flash_driver_generic::size_sector(), 1, (unsigned char *) &real_map_secondary);

        driver.read(1 + (i % 8) * sizeof(Flash_value_crc<int, crc<32>>), sizeof(int), (unsigned char *) &real_item_primary);
        driver.read(Flash_driver_generic::size_sector() + 1 + (i % 8) * sizeof(Flash_value_crc<int, crc<32>>), sizeof(int), (unsigned char *) &real_item_secondary);

        compare_map &= ~(0x80 >> (i % 8));

        REQUIRE(real_map_primary == compare_map);
        REQUIRE(real_map_secondary == compare_map);

        REQUIRE(real_item_primary == i);
        REQUIRE(real_item_secondary == i);

        if (i % 8 == 7) compare_map = 0xff;
    }

    driver.deinit();
}

TEST_CASE("value safe load healthy")
{
    Flash_driver_file driver;
    Flash_sector primary(0, &driver);
    Flash_sector secondary(1, &driver);

    Flash_value_safe<int, crc<32>, 8> value(0, primary, secondary);

    driver.init();

    unsigned char bitmap = 0x7f;
    int val = 0xdeadbeef;
    unsigned int sum = crc<32>(val);

    primary.at(0).write(bitmap);
    primary.write(val);
    primary.write(sum);

    secondary.at(0).write(bitmap);
    secondary.write(val);
    secondary.write(sum);

    auto status = value.load();
    REQUIRE(status == status::Success());
    REQUIRE(value.get() == 0xdeadbeef);

    driver.deinit();  
}

TEST_CASE("value safe load invalid crc primary")
{
    Flash_driver_file driver;
    Flash_sector primary(0, &driver);
    Flash_sector secondary(1, &driver);

    Flash_value_safe<int, crc<32>, 8> value(0, primary, secondary);

    driver.init();

    unsigned char bitmap = 0x7f;
    int val = 0xdeadbeef;
    unsigned int sum = crc<32>(val);

    primary.at(0).write(bitmap);
    primary.write(val);
    primary.write(val);

    secondary.at(0).write(bitmap);
    secondary.write(val);
    secondary.write(sum);

    auto status = value.load();
    REQUIRE(status == status::Success());
    REQUIRE(value.get() == 0xdeadbeef);

    driver.deinit();  
}

TEST_CASE("value safe load invalid crc no secondary")
{
    Flash_driver_file driver;
    Flash_sector primary(0, &driver);
    Flash_sector secondary(1, &driver);

    Flash_value_safe<int, crc<32>, 8> value(0, primary, secondary);

    driver.init();

    unsigned char bitmap = 0x7f;
    int val = 0xdeadbeef;
    unsigned int sum = crc<32>(val);

    primary.at(0).write(bitmap);
    primary.write(val);
    primary.write(val);

    auto status = value.load();
    REQUIRE(status != status::Success());

    driver.deinit();  
}

TEST_CASE("value safe load invalid crc primary recover")
{
    Flash_driver_file driver;
    Flash_sector primary(0, &driver);
    Flash_sector secondary(1, &driver);

    Flash_value_safe<int, crc<32>, 8> value(0, primary, secondary);

    driver.init();

    unsigned char bitmap = 0x3f;
    int val = 0xdeadbeef;
    unsigned int sum = crc<32>(val);

    primary.at(0).write(bitmap);
    primary.write(val);
    primary.write(sum);
    primary.write(val);
    primary.write(val);

    secondary.at(0).write(bitmap);
    secondary.write(val);
    secondary.write(sum);
    secondary.write(val);
    secondary.write(sum);

    auto status = value.load();
    REQUIRE(status == status::Success());
    REQUIRE(value.get() == 0xdeadbeef);

    driver.deinit();  
}


TEST_CASE("value safe load invalid crc primary recover no secondary")
{
    Flash_driver_file driver;
    Flash_sector primary(0, &driver);
    Flash_sector secondary(1, &driver);

    Flash_value_safe<int, crc<32>, 8> value(0, primary, secondary);

    driver.init();

    unsigned char bitmap = 0x3f;
    int val = 0xdeadbeef;
    unsigned int sum = crc<32>(val);

    primary.at(0).write(bitmap);
    primary.write(val);
    primary.write(sum);
    primary.write(val);
    primary.write(val);

    auto status = value.load();
    REQUIRE(status == status::Success());
    REQUIRE(value.get() == 0xdeadbeef);

    driver.deinit();  
}

TEST_CASE("value safe load invalid crc primary recover 1 secondary")
{
    Flash_driver_file driver;
    Flash_sector primary(0, &driver);
    Flash_sector secondary(1, &driver);

    Flash_value_safe<int, crc<32>, 8> value(0, primary, secondary);

    driver.init();

    unsigned char bitmap = 0x3f;
    unsigned char bitmap2 = 0x7f;
    int val = 0xdeadbeef;
    unsigned int sum = crc<32>(val);

    primary.at(0).write(bitmap);
    primary.write(val);
    primary.write(sum);
    primary.write(val);
    primary.write(val);

    secondary.at(0).write(bitmap2);
    secondary.write(val);
    secondary.write(sum);

    auto status = value.load();
    REQUIRE(status == status::Success());
    REQUIRE(value.get() == 0xdeadbeef);

    driver.deinit();  
}