#include "test.h"
#include "bitmap.h"


TEST_CASE("test bitmap")
{
    Bitmap<32> bitmap;

    bitmap.reset();
    
    REQUIRE(bitmap.is_empty() == true);

    for (int i = 0; i < 32; i++)
    {
        bitmap.set(i);
        REQUIRE(bitmap.count_set() == i + 1);
    }
    
    REQUIRE(bitmap.is_full() == true);
    
    int k = 2;

}