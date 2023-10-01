#ifndef FFL_PARTS_TRANSFORM_H_
#define FFL_PARTS_TRANSFORM_H_

#include <nn/ffl/FFLVec.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLPartsTransform
{
    FFLVec3 _0;
    FFLVec3 _c;
    FFLVec3 _18;
    FFLVec3 _24;
    FFLVec3 _30;
    FFLVec3 _3c;
    FFLVec3 _48;
}
FFLPartsTransform;
NN_STATIC_ASSERT(sizeof(FFLPartsTransform) == 0x54);

#ifdef __cplusplus
}
#endif

#endif // FFL_PARTS_TRANSFORM_H_
