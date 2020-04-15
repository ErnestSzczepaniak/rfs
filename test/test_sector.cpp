#include "test.h"
#include "sector.h"
#include "flash_driver_software.h"

TEST_CASE("asd")
{
    _init();

    unsigned char sample[] = {0x11, 0x22, 0x33};
    unsigned char rx[3];

    Sector s(0, &driver);

    s.write(2, 3, sample);
    s.read(2, 3, rx);

    
    sector.from(table).to(0x10).write(10);
    sector.from(0x0).to(table).read(20);

}
