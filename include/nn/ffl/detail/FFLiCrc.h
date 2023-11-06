#ifndef FFLI_CRC_H_
#define FFLI_CRC_H_

#include <nn/ffl/types.h>

u16 FFLiGetCRC16(const void* pData, u32 size);
void FFLiPutCRC16(void* pData, u32 size);
bool FFLiIsValidCRC16(const void* pData, u32 size);

#endif // FFLI_CRC_H_
