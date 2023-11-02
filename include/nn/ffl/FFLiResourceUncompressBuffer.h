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

    void* Buffer();

    void* TempBuffer();
    u32 TempBufferSize() const;

    static u32 GetBufferSize(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType);

private:
    void*   m_pBuffer;
    void*   m_pTempBuffer;
};
NN_STATIC_ASSERT(sizeof(FFLiResourceUncompressBuffer) == 8);

#endif // FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_
