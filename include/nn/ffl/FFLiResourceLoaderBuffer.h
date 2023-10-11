#ifndef FFLI_RESOURCE_LOADER_BUFFER_H_
#define FFLI_RESOURCE_LOADER_BUFFER_H_

#include <nn/ffl/FFLiResourceUncompressBuffer.h>

class FFLiFsCommandBuffer;

class FFLiResourceLoaderBuffer
{
public:
    FFLiResourceLoaderBuffer(const FFLiResourceManager* pResourceManager, FFLiBufferAllocator* pAllocator, FFLResourceType resourceType);
    ~FFLiResourceLoaderBuffer();

    FFLiResourceUncompressBuffer& GetUncompressBuffer()
    {
        return m_ResourceUncompressBuffer;
    }

    const FFLiResourceUncompressBuffer& GetUncompressBuffer() const
    {
        return m_ResourceUncompressBuffer;
    }

    FFLiFsCommandBuffer* GetCommandBuffer() const
    {
        return m_pFsCommandBuffer;
    }

private:
    FFLiResourceUncompressBuffer    m_ResourceUncompressBuffer;
    FFLiFsCommandBuffer*            m_pFsCommandBuffer;
};
NN_STATIC_ASSERT(sizeof(FFLiResourceLoaderBuffer) == 0xC);

#endif // FFLI_RESOURCE_LOADER_BUFFER_H_
