#include "test.h"
#include "flash_ring_buffer.h"
#include "flash_driver_file.h"
#include "crc.h"

TEST_CASE("buffer reset")
{
    Flash_driver_file driver;
    Flash_ring_buffer<int, crc<32>> buffer;
    buffer.init(&driver);

    buffer.reset();

    REQUIRE(buffer.size_actual() == 0);

    auto size = ((Flash_driver_generic::number_sector() - 2) * Flash_driver_generic::size_sector()) / sizeof(Flash_value_crc<int, crc<32>>);

    REQUIRE(buffer.size_max() == size);

    driver.deinit();
}

TEST_CASE("buffer push")
{
    Flash_driver_file driver;
    Flash_ring_buffer<int, crc<32>> buffer;
    buffer.init(&driver);

    buffer.reset();

    for (int i = 0; i < buffer.size_max(); i++)
    {
        buffer.push(i);
    }
    
    REQUIRE(buffer.size_actual() == buffer.size_max());

    driver.deinit();
}

TEST_CASE("buffer pop")
{
    Flash_driver_file driver;
    Flash_ring_buffer<int, crc<32>> buffer;
    buffer.init(&driver);

    buffer.reset();

    for (int i = 0; i < buffer.size_max(); i++)
    {
        buffer.push(i);
    }
    
    for (int i = 0; i < buffer.size_max(); i++)
    {
        buffer.pop();
    }

    REQUIRE(buffer.size_actual() == 0);

    driver.deinit();
}

TEST_CASE("buffer at")
{
    Flash_driver_file driver;
    Flash_ring_buffer<int, crc<32>> buffer;
    buffer.init(&driver);

    buffer.reset();

    for (int i = 0; i < buffer.size_max(); i++)
    {
        buffer.push(i);
    }
    
    for (int i = 0; i < buffer.size_max(); i++)
    {
        REQUIRE(buffer.at(i).value == i);
    }

    driver.deinit();
}

TEST_CASE("buffer overflow")
{
    Flash_driver_file driver;
    Flash_ring_buffer<int, crc<32>> buffer;
    buffer.init(&driver);

    buffer.reset();

    for (int i = 0; i < buffer.size_max() + 1; i++)
    {
        buffer.push(i);
    }
    
    auto items = Flash_driver_generic::size_sector() / sizeof(Flash_value_crc<int, crc<32>>);
    auto size = buffer.size_max() - items + 1;

    REQUIRE(buffer.size_actual() == size);

    for (int i = 0; i < buffer.size_actual(); i++)
    {
        REQUIRE(buffer.at(i).value == i + items);
    }
    
    driver.deinit();   
}

struct Type
{
    int k;
    const char * w;
}; /* structure: Type */


TEST_CASE("buffer other typ")
{
    Flash_driver_file driver;
    Flash_ring_buffer<int, crc<32>> buffer;
    buffer.init(&driver);

    buffer.reset();

    driver.deinit();
}