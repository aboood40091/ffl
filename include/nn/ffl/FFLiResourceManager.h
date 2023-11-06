#ifndef FFLI_RESOURCE_MANAGER_H_
#define FFLI_RESOURCE_MANAGER_H_

#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiShapePartsType.h>
#include <nn/ffl/FFLiTexturePartsType.h>

#include <nn/ffl/detail/FFLiResourceCache.h>

struct  FFLiResourceHeader;
struct  FFLiResourceMultiHeader;

class FFLiResourceManager
{
public:
    FFLiResourceManager(FFLiResourceMultiHeader* pHeader);
    ~FFLiResourceManager();

    static const char* GetRelativeResourcePath(FFLResourceType resourceType, bool LG);
    static FFLResult GetResourcePath(char* pDst, u32 size, FFLResourceType resourceType, bool LG);

    FFLResult AfterConstruct();

    FFLResult LoadResourceHeader();
    FFLResult LoadResourceHeaderImpl();

    FFLResult AttachCache(void* pData, u32 size, FFLResourceType resourceType);
    bool IsCached() const;

    FFLiResourceHeader* Header(FFLResourceType resourceType) const;
    FFLiResourceHeader* HeaderFromCache(FFLResourceType resourceType) const;
    FFLiResourceHeader* HeaderFromFile(FFLResourceType resourceType) const;

    u32 GetTextureAlignedMaxSize(FFLResourceType resourceType, FFLiTexturePartsType partsType) const;
    u32 GetShapeAlignedMaxSize(FFLResourceType resourceType, FFLiShapePartsType partsType) const;

    bool IsValid(FFLResourceType resourceType) const;
    bool IsExpand(FFLResourceType resourceType) const;

    u32 GetUncompressBufferSize(FFLResourceType resourceType) const;

    FFLiResourceCache& GetResourceCache()
    {
        return m_ResourceCache;
    }

    const FFLiResourceCache& GetResourceCache() const
    {
        return m_ResourceCache;
    }

    const char* GetPath(FFLResourceType resourceType) const;

private:
    FFLiResourceMultiHeader*    m_pResourceMultiHeader;
    void*                       _4; // Deleted
    FFLiResourceCache           m_ResourceCache;
    char                        m_Path[FFL_RESOURCE_TYPE_MAX][FFL_PATH_MAX_LEN];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceManager) == 0x218);

#endif // FFLI_RESOURCE_MANAGER_H_
