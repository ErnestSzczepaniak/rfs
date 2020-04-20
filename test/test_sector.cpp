#include "test.h"
#include "flash_sector.h"
#include "flash_driver_software.h"



TEST_CASE("asd")
{
    _init();

    unsigned int as = 0xffffffff;
    char key = 0x55;

    Flash_sector s(0, &driver);

    s.reset();


    auto [status, success] = s.at(0).verify_with(as);

}
