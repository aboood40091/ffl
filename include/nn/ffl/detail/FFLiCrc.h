#ifndef FFLI_CRC_H_
#define FFLI_CRC_H_

#include <nn/ffl/types.h>

void FFLiPutCRC16(u16 base, void* pData, u32 size);
bool FFLiIsValidCRC16(const void* pData, u32 size);

#endif // FFLI_CRC_H_
