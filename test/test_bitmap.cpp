#include "test.h"
#include "bitmap.h"


TEST_CASE("test bitmap")
{
    Bitmap<32> bitmap;

    bitmap.reset(false);
    
    REQUIRE(bitmap.is_full_of(0) == true);

    bitmap.reset(true);

    REQUIRE(bitmap.is_full_of(true) == true);


    auto w = bitmap.allocate(false);

    
    int k = 2;

}