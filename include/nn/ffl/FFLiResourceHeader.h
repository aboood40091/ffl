#ifndef FFLI_RESOURCE_HEADER_H_
#define FFLI_RESOURCE_HEADER_H_

#include <nn/ffl/types.h>

#define FFLI_RESOURCE_MULTI_HEADER_SIZE (0x9400)

struct FFLiResourceMultiHeader
{
    union
    {
        u8 data[FFLI_RESOURCE_MULTI_HEADER_SIZE];
        u32 data32[FFLI_RESOURCE_MULTI_HEADER_SIZE / sizeof(u32)];
    };
};
NN_STATIC_ASSERT(sizeof(FFLiResourceMultiHeader) == FFLI_RESOURCE_MULTI_HEADER_SIZE);

#endif // FFLI_RESOURCE_HEADER_H_
