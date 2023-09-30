#ifndef FFL_PARTS_TRANSFORM_H_
#define FFL_PARTS_TRANSFORM_H_

#include <nn/ffl/FFLVec.h>

#include <cafe/mtx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLPartsTransform
{
    FFLVec3 _0;
    Mtx     _c;
    u32     _3c[24 / sizeof(u32)];
}
FFLPartsTransform;
NN_STATIC_ASSERT(sizeof(FFLPartsTransform) == 0x54);

#ifdef __cplusplus
}
#endif

#endif // FFL_PARTS_TRANSFORM_H_
