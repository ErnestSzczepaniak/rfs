#include "test.h"
#include "sector.h"
#include "flash_driver_software.h"

TEST_CASE("asd")
{
    _init();

    unsigned int as = 0x12345678;

    Sector s(1, &driver);

    for (int i = 0; i < 16; i++)
    {
        s.write(as);
    }
    
    s.cell(10).clear(8, true);
}
