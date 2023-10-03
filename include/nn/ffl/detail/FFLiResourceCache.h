#ifndef FFLI_RESOURCE_CACHE_H_
#define FFLI_RESOURCE_CACHE_H_

#include <nn/ffl/FFLResourceType.h>

class FFLiResourceCache
{
public:
    FFLiResourceCache();
    ~FFLiResourceCache();

private:
    struct
    {
        const void* data;
        u32 size;
    }   m_Res[FFL_RESOURCE_TYPE_MAX];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceCache) == 0x10);

#endif // FFLI_RESOURCE_CACHE_H_
