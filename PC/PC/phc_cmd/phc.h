// PHC protocol definitions

#ifndef _PHC_H

#include "scalar_types.h"


 // CRC across a buffer
uint16_t crc16(const uint8_t* buf, int n);

// do the HDLC-style padding, dst and src can be identical
int pad_buffer(uint8_t* buf_dst, const uint8_t* buf_src, int* size);

// undo the HDLC-style padding, dst and src can be identical
int unpad_buffer(uint8_t* buf_dst, const uint8_t* buf_src, int* size);

// convert a module bus command to a (RS232) STM bus command
// both buffer pointers may point to the same buffer, inplace is allowed
int module_to_stm(uint8_t* buffer_stm, const uint8_t* buffer_mod, int* size);

// convert a (RS232) STM bus command bus command to a module bus command
// both buffer pointers may point to the same buffer, inplace is allowed
int stm_to_module(uint8_t* buffer_mod, const uint8_t* buffer_stm, int* size);



#endif // #ifndef _PHC_H
