#include "test.h"
#include "flash_driver_generic.h"
#include "flash_value_crc.h"
#include "crc.h"

TEST_CASE("test_case_name")
{
    int bytes = 1;
    int size_sector = Flash_driver_generic::size_sector() / 2;
    int size_index = sizeof(Flash_value_crc<int, crc<32>>);

    while(true)
    {
        auto span = bytes * 8 * size_index;

        if ((size_sector - bytes - span) < 0)
        {
            bytes--;
            break;
        }
        else bytes++;   
    }

    printf("%d bytes possible\n", bytes);
}