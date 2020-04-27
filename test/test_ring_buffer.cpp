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
    unsigned char s[] = {0};


    _init();
    Log log;

    Flash_ring_buffer<Log> ring;

    ring.init(&driver);
    ring.reset();

    for (int i = 0; i < 164; i++)
    {
        memset(&log, i + 1, sizeof(Log));

        ring.push(log);
        ring.pop();
        printf("size = %d\n", ring.size_actual());
    }
    
    for (int i = 0; i < 64; i++)
    {
        memset(&log, i + 1, sizeof(Log));

        ring.push(log);
        printf("size = %d\n", ring.size_actual());
    }

    for (int i = 0; i < 64; i++)
    {
        auto [stauts, l] = ring.at(i);
        Log temp;
        memset(&temp, i + 1, sizeof(Log));
        REQUIRE(memcmp(&temp, &l, sizeof(Log)) == 0);
        printf("size = %d\n", ring.size_actual());
    }

    
}