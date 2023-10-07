#ifndef FFL_RESOURCE_DESC_H_
#define FFL_RESOURCE_DESC_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLResourceDesc
{
    const void* pData;
    u32         _4[4 / sizeof(u32)];
    u32         size;
}
FFLResourceDesc;
NN_STATIC_ASSERT(sizeof(FFLResourceDesc) == 0xC);

#ifdef __cplusplus
}
#endif

#endif // FFL_RESOURCE_DESC_H_