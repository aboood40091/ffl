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
NN_STATIC_ASSERT(sizeof(FFLBoundingBox) == FFL_BOUNDING_BOX_SIZE);
NN_STATIC_ASSERT(alignof(FFLBoundingBox) == FFL_BOUNDING_BOX_ALIGN);

#ifdef __cplusplus
}
#endif

#endif // FFL_BOUNDING_BOX_H_
