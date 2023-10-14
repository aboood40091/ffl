#include <nn/ffl/FFLiResourceUtil.h>

#include <nn/ffl/detail/FFLiResourceCache.h>

#include <cstring>

FFLiResourceCache::FFLiResourceCache()
{
    std::memset(m_Res, 0, sizeof(m_Res));
}

FFLiResourceCache::~FFLiResourceCache()
{
}

FFLResult FFLiResourceCache::Attach(const void* pData, u32 size, FFLResourceType resourceType)
{
    FFLiResourceHeader* pHeader = (FFLiResourceHeader*)pData;

    FFLResult result = FFLiIsVaildResourceHeader(pHeader);
    if (result != FFL_RESULT_OK)
        return result;

    m_Res[resourceType].pHeader = pHeader;
    m_Res[resourceType].size = size;

    return FFL_RESULT_OK;
}

bool FFLiResourceCache::IsAttached() const
{
    for (u32 i = 0; i < FFL_RESOURCE_TYPE_MAX; i++)
        if (IsValid(FFLResourceType(i)))
            return true;

    return false;
}

bool FFLiResourceCache::IsValid(FFLResourceType resourceType) const
{
    return m_Res[resourceType].pHeader != NULL;
}

FFLiResourceHeader* FFLiResourceCache::Header(FFLResourceType resourceType) const
{
    return m_Res[resourceType].pHeader;
}
