#ifndef FFL_RESOURCE_DESC_H_
#define FFL_RESOURCE_DESC_H_

#include <nn/ffl/FFLResourceType.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLResourceDesc
{
    void*   pData[FFL_RESOURCE_TYPE_MAX];
    u32     size[FFL_RESOURCE_TYPE_MAX];
}
FFLResourceDesc;
NN_STATIC_ASSERT(sizeof(FFLResourceDesc) == 0x10);

#ifdef __cplusplus
}
#endif

#endif // FFL_RESOURCE_DESC_H_
