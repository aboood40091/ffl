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
    BOOL            _10;    // Removed
}
FFLCharModelDesc;
NN_STATIC_ASSERT(sizeof(FFLCharModelDesc) == 0x14);

#ifdef __cplusplus
}
#endif

#endif // FFL_CHAR_MODEL_DESC_H_
