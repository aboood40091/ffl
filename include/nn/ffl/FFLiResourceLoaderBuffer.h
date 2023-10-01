#ifndef FFLI_RESOURCE_LOADER_BUFFER_H_
#define FFLI_RESOURCE_LOADER_BUFFER_H_

#include <nn/ffl/FFLiResourceUncompressBuffer.h>

class FFLiResourceLoaderBuffer
{
public:
    FFLiResourceLoaderBuffer(const FFLiResourceManager* pResourceManager, FFLiBufferAllocator* pAllocator, FFLResourceType resourceType);
    ~FFLiResourceLoaderBuffer();

private:
    FFLiResourceUncompressBuffer    m_ResourceUncompressBuffer;
    void*                           m_pBuffer;
};
NN_STATIC_ASSERT(sizeof(FFLiResourceLoaderBuffer) == 0xC);

#endif // FFLI_RESOURCE_LOADER_BUFFER_H_
