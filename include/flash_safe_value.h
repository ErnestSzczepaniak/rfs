// #ifndef _flash_safe_value_h
// #define _flash_safe_value_h

// /**
//  * @file	flash_safe_value.h
//  * @author	en2
//  * @date	23-04-2020
//  * @brief	
//  * @details	
// **/

// #include "flash_filesystem_value.h"

// template<typename T>
// class Flash_safe_value
// {
// public:
//     Flash_safe_value(int address, Flash_sector & primary, Flash_sector & secondary);
//     ~Flash_safe_value();

//     T & get();
//     Flash_safe_value & set(T value);

//     Status load();
//     Status store();

// protected:
//     Status _copy_from_primary();
//     Status _copy_from_secondary();
//     Status _recover_from_primary();
//     Status _recover_from_secondary();

// private:
//     int _address;

//     Flash_sector & _sector_primary;
//     Flash_sector & _sector_secondary;

//     Flash_filesystem_value<T> _value_primary;
//     Flash_filesystem_value<T> _value_secondary;

// }; /* class: Flash_safe_value */

// template<typename T> 
// Flash_safe_value<T>::Flash_safe_value(int address, Flash_sector & primary, Flash_sector & secondary)
// :
// _address(address),
// _sector_primary(primary),
// _sector_secondary(secondary)
// {

// }

// template<typename T> 
// Flash_safe_value<T>::~Flash_safe_value()
// {

// }

// template<typename T> 
// T & Flash_safe_value<T>::get()
// {   
//     return _value_primary.get();
// }

// template<typename T> 
// Flash_safe_value<T> & Flash_safe_value<T>::set(T value)
// {
//     _value_primary.set(value);
//     _value_secondary.set(value);

//     return *this;
// }

// template<typename T> 
// Status Flash_safe_value<T>::load()
// {
//     auto status_primary = _value_primary.load_from(_sector_primary.at(_address));
//     auto status_secondary = _value_secondary.load_from(_sector_secondary.at(_address));

//     // both secors empty or valid crc
//     if (status_primary == true && status_secondary == true)
//     {
//         // both not empty
//         if (_value_primary.usage() > 0 && _value_secondary.usage() > 0)
//         {
//             // second is behind, copy from first
//             if (_value_primary.get() != _value_secondary.get()) return _copy_from_primary();
//             // both at same point
//             else return true;
//         }
//         // first empty, second not empty
//         else if (_value_primary.usage() == 0 && _value_secondary.usage() > 0) return _copy_from_secondary();
//         // second empty, first not empty
//         else if (_value_primary.usage() > 0 && _value_primary.usage() == 0) return _copy_from_primary();
//         // both empty
//         else return warning::value::Empty();
//     }
//     // first empty or valid, second not not empty and not valid
//     else if (status_primary == true && status_secondary == error::frame::Crc_mismatch()) // seconday 
//     {
//         // first valid
//         if (_value_primary.usage() > 0) return _copy_from_primary();
//         // first empty
//         else return _recover_from_secondary();
//     }
//     // first not empty and not valid, second empty ov valid
//     else if (status_primary == error::frame::Crc_mismatch() && status_secondary == true)
//     {
//         // second valid
//         if (_value_secondary.usage() > 0) return _copy_from_secondary();
//         // second empty
//         else return _recover_from_primary();
//     }
//     // both not empty and not valid
//     else if (status_primary == error::frame::Crc_mismatch() && status_secondary == error::frame::Crc_mismatch())
//     {
//         // primary is ahead
//         if (_value_primary.usage() > _value_secondary.usage()) return _recover_from_primary();
//         // secondary is ahead
//         else return _recover_from_secondary();
//     }

//     return false;
// }

// template<typename T> 
// Status Flash_safe_value<T>::store()
// {
//     if (auto status_primary = _value_primary.store_to(_sector_primary.at(_address)); status_primary == false) return status_primary;
//     if (auto status_secondary = _value_secondary.store_to(_sector_secondary.at(_address)); status_secondary == false) return status_secondary;

//     return true;
// }

// /* ---------------------------------------------| info |--------------------------------------------- */

// template<typename T> 
// Status Flash_safe_value<T>::_copy_from_primary()
// {
//     _value_secondary.set(_value_primary.get());
//     return _value_secondary.store_to(_sector_secondary.at(_address));
// }

// template<typename T> 
// Status Flash_safe_value<T>::_copy_from_secondary()
// {
//     _value_primary.set(_value_secondary.get());
//     return _value_primary.store_to(_sector_primary.at(_address));
// }

// template<typename T> 
// Status Flash_safe_value<T>::_recover_from_primary()
// {
//     if (auto status_recover = _value_primary.recover_from(_sector_primary.at(_address)); status_recover == false) return status_recover;

//     _value_secondary.set(_value_primary.get());
//     return _value_secondary.store_to(_sector_primary.at(_address)); 
// }

// template<typename T> 
// Status Flash_safe_value<T>::_recover_from_secondary()
// {
//     if (auto status_recover = _value_secondary.recover_from(_sector_secondary.at(_address)); status_recover == false) return status_recover;

//     _value_primary.set(_value_secondary.get());
//     return _value_primary.store_to(_sector_primary.at(_address));
// }

// #endif /* define: flash_safe_value_h */