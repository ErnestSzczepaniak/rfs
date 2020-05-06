#ifdef build_platform_host

#define CATCH_CONFIG_MAIN
#include "build.h"
#include "test.h"

#else

#include "build.h"
#include "flash_driver_qspi.h"
#include "flash_ring_buffer.h"
#include "crc.h"

static constexpr auto size_test = 5;

struct Data
{
    unsigned char payload[size_test];
}; /* structure: Data */


int main()
{
    Flash_driver_qspi driver;
    driver.init();

    int size_actual, size_max;

    // Data tx, rx;
    
    // Flash_sector s(0, &driver);

    // for (int i = 0; i < sizeof(Data); i++)
    // {
    //     tx.payload[i] = i;
    // }

    // s.clear();
    // s.at(1).write(tx);
    // auto result = s.at(1).read<Data>();
    
    Flash_ring_buffer<int, crc<32>> buffer(&driver);
    // // int tx[128], Data[128];


    
    // /* ---------------------------------------------| info |--------------------------------------------- */

    int tx[128], rx[128];

    //auto status = buffer.reset();
    auto status = buffer.init();

    // auto size_actual = buffer.size_actual();
    // auto size_max = buffer.size_max();

    for (int i = 0; i < 128; i++)
    {
        buffer.push(tx[i]);
    }

    size_actual = buffer.size_actual();

    // /* ---------------------------------------------| info |--------------------------------------------- */

    status = buffer.init();

    size_actual = buffer.size_actual();
    size_max = buffer.size_max();

    for (int i = 0; i < 128; i++)
    {
        rx[i] = buffer.at(i).value;
    }

    // /* ---------------------------------------------| info |--------------------------------------------- */

    // bool same = true;

    // for (int i = 0; i < 128; i++)
    // {
    //     if (tx[i] != Data[i])
    //     {
    //         same = false;
    //         break;
    //     }
    // }
    
    for (int i = 0; i < 128; i++)
    {
        buffer.pop();
    }
    
    size_actual = buffer.size_actual();

    while(1);
}

#endif
