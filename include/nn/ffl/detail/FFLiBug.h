#ifndef FFLI_BUG_H_
#define FFLI_BUG_H_

#include <nn/ffl/types.h>

void FFLiBugEndianSwap(void* ptr, u32 size);

u32 FFLiBugCanSwapSize(u32 size);

void* FFLiBugVgtFixedIndexPtr(void* ptr);
u32 FFLiBugCanVgtFixedIndexSize(u32 size);

#endif // FFLI_BUG_H_
