// #ifndef _flash_driver_qspi_h
// #define _flash_driver_qspi_h

// /**
//  * @file	flash_driver_qspi.h
//  * @author	en2
//  * @date	04-05-2020
//  * @brief	
//  * @details	
// **/

// #ifdef build_platform_target

// #include "flash_driver_generic.h"

// class Flash_driver_qspi : public Flash_driver_generic   
// {
//     static constexpr auto size_buffer = Flash_driver_generic::size_sector() / 4 + 2;

// public:
//     bool init();
//     bool deinit();

//     bool read(int, int, unsigned char *);
//     bool write(int, int, unsigned char *);
//     bool erase(int, int);   

// protected: 
//     bool _write_partial(int address, int size, unsigned char * data);

// private:
//     static unsigned int _buffer[size_buffer];

// }; /* class: Flash_driver_qspi */

// #endif

// #endif /* define: flash_driver_qspi_h */