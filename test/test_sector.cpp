#include "test.h"
#include "flash_sector.h"
#include "flash_driver_software.h"

TEST_CASE("asd")
{
    _init();

    unsigned int as[2] = {0x12345678, 0};
    char key = 0x55;

    Flash_sector s(0, &driver);

    s.reset();

    auto bytest = s.at(0).write_from(as);
    auto wqe = s.at(3).replace_with(key);

}
