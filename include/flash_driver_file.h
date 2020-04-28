#ifndef _flash_driver_file_h
#define _flash_driver_file_h

/**
 * @file	flash_driver_file.h
 * @author	en2
 * @date	28-04-2020
 * @brief	
 * @details	
**/

#include "flash_driver_generic.h"
#include <fstream>

class Flash_driver_file : public Flash_driver_generic
{
    /**
     * @class	Flash_driver_file
     * @brief	
     * @details	
    **/

public:
    bool init();
    bool deinit();
    
    bool read(int, int, unsigned char *);
    bool write(int, int, unsigned char *);
    bool erase(int, int);

private:
    std::fstream _stream;

}; /* class: Flash_driver_file */


#endif /* define: flash_driver_file_h */