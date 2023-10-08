#ifndef FFLI_MIP_MAP_UTIL_H_
#define FFLI_MIP_MAP_UTIL_H_

#include <nn/ffl/types.h>

s32 FFLiGetMipMapNum(s32 width, s32 height);
u32 FFLiGetMipMapLevelSize(u32 size, u32 mipLevel);

#endif // FFLI_MIP_MAP_UTIL_H_
