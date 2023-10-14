#ifndef FFLI_RESOURCE_CACHE_H_
#define FFLI_RESOURCE_CACHE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

class FFLiResourceHeader;

class FFLiResourceCache
{
public:
    FFLiResourceCache();
    ~FFLiResourceCache();

    FFLResult Attach(const void* pData, u32 size, FFLResourceType resourceType);
    bool IsAttached() const;

    bool IsValid(FFLResourceType resourceType) const;

    FFLiResourceHeader* Header(FFLResourceType resourceType) const;

private:
    struct
    {
        FFLiResourceHeader* pHeader;
        u32                 size;
    }   m_Res[FFL_RESOURCE_TYPE_MAX];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceCache) == 0x10);

#endif // FFLI_RESOURCE_CACHE_H_
