#ifndef FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_
#define FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_

#include <nn/ffl/FFLResourceType.h>

class FFLiBufferAllocator;
class FFLiResourceManager;

class FFLiResourceUncompressBuffer
{
public:
    FFLiResourceUncompressBuffer(const FFLiResourceManager* pResourceManager, FFLiBufferAllocator* pAllocator, FFLResourceType resourceType);
    ~FFLiResourceUncompressBuffer();

    static u32 GetBufferSize(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType);

private:
    void*   m_pBuffer;
    void*   m_pTempBuffer;
};

#endif // FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_
