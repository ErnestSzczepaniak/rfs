#include "test.h"
#include "sector.h"
#include "flash_driver_software.h"

TEST_CASE("asd")
{
    _init();

    unsigned int tab = 0x12345678;

    for (int i = 0; i < 20; i++)
    {
        driver.write(i * 4, 4, &tab); 
    }
    
    driver.erase(0x40);
    
}
