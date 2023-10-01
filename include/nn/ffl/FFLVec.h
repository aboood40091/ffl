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

#ifdef __cplusplus
}
#endif

#endif // FFL_VECTOR_H_
