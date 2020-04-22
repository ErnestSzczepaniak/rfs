#include "test.h"
#include "flash_bitmap.h"

TEST_CASE("test bitmap")
{
    Flash_bitmap bitmap;

    bitmap._bitmap[0] = 0x0f;

    REQUIRE(bitmap._busy() == 4);
    REQUIRE(bitmap.allocate() == 4);
}