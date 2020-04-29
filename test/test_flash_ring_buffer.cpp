#include "test.h"
#include "flash_ring_buffer.h"
#include "flash_driver_file.h"
#include "crc.h"

TEST_CASE("buffer init")
{
    Flash_driver_file driver;
    Flash_ring_buffer<int, crc<32>> buffer(&driver);

    driver.init();

    buffer.reset();

    for (int i = 0; i < 191; i++)
    {
        buffer.push(i);
    }
    
    auto s = buffer.size_actual();

    buffer.push(2);

    driver.deinit();
}
