#include "test.h"
#include "flash_safe_value.h"
#include "flash_driver_software.h"

TEST_CASE("test safe value")
{
    _init();

    Flash_sector sector0, sector1;

    sector0.number(0).driver(&driver).at(0).clear();
    sector1.number(1).driver(&driver).at(0).clear();

    Flash_safe_value<int> flash_value(0, sector0, sector1);

    auto result = flash_value.load();

    result = flash_value.set(100).store();

    printf("Type: [%s], Category: [%s], Brief: [%s]\n", result.type(), result.category(), result.brief());
    printf("File: [%s], Function [%s], Line [%d]\n", result.file(), result.function(), result.line());

    //flash_value.set(10);

    //flash_value.store();

}