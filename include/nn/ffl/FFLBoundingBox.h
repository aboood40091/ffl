#ifndef FFL_BOUNDING_BOX_H_
#define FFL_BOUNDING_BOX_H_

#include <nn/ffl/FFLVec.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLBoundingBox
{
    FFLVec3 min;
    FFLVec3 max;
}
FFLBoundingBox;
NN_STATIC_ASSERT(sizeof(FFLBoundingBox) == 0x18);

#ifdef __cplusplus
}
#endif

#endif // FFL_BOUNDING_BOX_H_
