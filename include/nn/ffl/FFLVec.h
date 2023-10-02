#ifndef FFL_VECTOR_H_
#define FFL_VECTOR_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLVec2
{
    f32 x;
    f32 y;
}
FFLVec2;
NN_STATIC_ASSERT(sizeof(FFLVec2) == 8);

typedef struct FFLVec3
{
    f32 x;
    f32 y;
    f32 z;
}
FFLVec3;
NN_STATIC_ASSERT(sizeof(FFLVec3) == 0xC);

typedef struct FFLVec4
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
}
FFLVec4;
NN_STATIC_ASSERT(sizeof(FFLVec4) == 0x10);

typedef struct FFLiSnorm10_10_10_2
{
    u32 w   :  2;   // (MSB)
    u32 z   : 10;
    u32 y   : 10;
    u32 x   : 10;   // (LSB)
}
FFLiSnorm10_10_10_2;
NN_STATIC_ASSERT(sizeof(FFLiSnorm10_10_10_2) == 4);

typedef struct FFLiSnorm8_8_8_8
{
    s8 x;
    s8 y;
    s8 z;
    s8 w;
}
FFLiSnorm8_8_8_8;
NN_STATIC_ASSERT(sizeof(FFLiSnorm8_8_8_8) == 4);

#ifdef __cplusplus
}
#endif

#endif // FFL_VECTOR_H_
