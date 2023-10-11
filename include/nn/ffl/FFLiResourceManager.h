#ifndef FFLI_RESOURCE_MANAGER_H_
#define FFLI_RESOURCE_MANAGER_H_

#include <nn/ffl/FFLiShapePartsType.h>

#include <nn/ffl/detail/FFLiResourceCache.h>

class   FFLiFsClient;
struct  FFLiFsCommandBuffer;
struct  FFLiResourceHeader;
struct  FFLiResourceMultiHeader;

class FFLiResourceManager
{
public:
    FFLiResourceManager(FFLiResourceMultiHeader* pHeader, FFLiFsClient* pClient);
    ~FFLiResourceManager();

    FFLResult AfterConstruct();

    const char* GetPath(FFLResourceType resourceType) const;

    FFLResult LoadResourceHeader(FFLiFsCommandBuffer* pCommandBuffer);
    FFLResult AttachCache(const void* pData, u32 size, FFLResourceType resourceType);

    FFLiResourceHeader* Header(FFLResourceType resourceType) const;

    u32 GetShapeAlignedMaxSize(FFLResourceType resourceType, FFLiShapePartsType partsType) const;

    bool IsCached() const;

    bool IsValid(FFLResourceType resourceType) const;
    bool IsExpand(FFLResourceType resourceType) const;

    FFLiFsClient* GetClient() const
    {
        return m_pFsClient;
    }

    FFLiResourceCache& GetResourceCache()
    {
        return m_ResourceCache;
    }

    const FFLiResourceCache& GetResourceCache() const
    {
        return m_ResourceCache;
    }

private:
    FFLiResourceMultiHeader*    m_pResourceMultiHeader;
    FFLiFsClient*               m_pFsClient;
    FFLiResourceCache           m_ResourceCache;
    char                        m_Path[FFL_RESOURCE_TYPE_MAX][FFL_PATH_MAX_LEN];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceManager) == 0x218);

#endif // FFLI_RESOURCE_MANAGER_H_
