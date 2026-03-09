#ifndef FFL_CHAR_MODEL_DESC_H_
#define FFL_CHAR_MODEL_DESC_H_

#include <nn/ffl/FFLResolution.h>
#include <nn/ffl/FFLResourceType.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLCharModelDesc
{
    FFLResolution   resolution;
    u32             expressionFlag;
    u32             modelFlag;
    FFLResourceType resourceType;
    BOOL            _10;    // Deleted
}
FFLCharModelDesc;
NN_STATIC_ASSERT(sizeof(FFLCharModelDesc) == FFL_CHAR_MODEL_DESC_SIZE);
NN_STATIC_ASSERT(alignof(FFLCharModelDesc) == FFL_CHAR_MODEL_DESC_ALIGN);

#ifdef __cplusplus
}
#endif

#endif // FFL_CHAR_MODEL_DESC_H_
