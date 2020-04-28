#include "test.h"
#include "flash_bitmap.h"
#include "string.h"

TEST_CASE("size assertion")
{
    Flash_bitmap<8> bitmap_8;
    Flash_bitmap<16> bitmap_16;
    Flash_bitmap<24> bitmap_24;
    Flash_bitmap<32> bitmap_32;
    Flash_bitmap<64> bitmap_64;

    REQUIRE(sizeof(bitmap_8) == 1);
    REQUIRE(sizeof(bitmap_16) == 2);
    REQUIRE(sizeof(bitmap_24) == 3);
    REQUIRE(sizeof(bitmap_32) == 4);
    REQUIRE(sizeof(bitmap_64) == 8);
}

TEST_CASE("bitmap fill")
{
    Flash_bitmap<32> bitmap;
    unsigned char compare[sizeof(bitmap)];

    memset(compare, 0xff, sizeof(compare));
    bitmap.fill(true);

    REQUIRE(memcmp(&bitmap, compare, sizeof(compare)) == 0);

    memset(compare, 0, sizeof(compare));
    bitmap.fill(false);

    REQUIRE(memcmp(&bitmap, compare, sizeof(compare)) == 0);
}

TEST_CASE("bitmap is full")
{
    Flash_bitmap<32> bitmap;

    bitmap.fill(true);
    REQUIRE(bitmap.is_full(true) == true);
    REQUIRE(bitmap.is_full(false) == false);

    bitmap.fill(false);
    REQUIRE(bitmap.is_full(false) == true);
    REQUIRE(bitmap.is_full(true) == false);
}

TEST_CASE("bitmap set from false")
{
    Flash_bitmap<32> bitmap;

    bitmap.fill(false);

    for (int i = 0; i < 32; i++)
    {
        bitmap.set(i, true);
        REQUIRE(bitmap.get(i) == true);
    }

    REQUIRE(bitmap.is_full(true) == true);
}

TEST_CASE("bitmap clear from true")
{
    Flash_bitmap<32> bitmap;

    bitmap.fill(true);

    for (int i = 0; i < 32; i++)
    {
        bitmap.set(i, false);
        REQUIRE(bitmap.get(i) == false);
    }

    REQUIRE(bitmap.is_full(false) == true);
}

TEST_CASE("bitmap count")
{
    Flash_bitmap<32> bitmap;

    bitmap.fill(false);

    REQUIRE(bitmap.count(false) == 32);
    REQUIRE(bitmap.count(true) == 0);

    for (int i = 0; i < 32; i++)
    {
        bitmap.set(i, true);
        REQUIRE(bitmap.count(true) == i + 1);
        REQUIRE(bitmap.count(false) == 32 - i - 1);
    }
    
    REQUIRE(bitmap.count(false) == 0);
    REQUIRE(bitmap.count(true) == 32);
}