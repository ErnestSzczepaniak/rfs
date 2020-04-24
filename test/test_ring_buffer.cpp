#include "test.h"
#include "flash_ring_buffer.h"
#include "flash_driver_software.h"

struct Log
{
    unsigned char data[12];
}; /* structure: Log */

static constexpr auto logs_per_sector = size_sector / sizeof(Flash_crc_value<Log>);

TEST_CASE("test ring buffer")
{
    _init();
    Log log;

    for (int j = 0; j < 12; j++)
    {
        log.data[j] = j;
    }
    
    Flash_ring_buffer<Log> ring;

    ring.init(&driver);
    ring.reset();

    for (int i = 0; i < ring.size_max(); i++)
    {
        memset(&log, i + 1, sizeof(Log));

        ring.push(log);
        printf("size = %d\n", ring.size_actual());
    }

    ring.push(log);

    auto [stauts, l] = ring.pop();
}