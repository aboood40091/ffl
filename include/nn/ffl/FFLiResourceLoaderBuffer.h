#ifndef FFLI_RESOURCE_LOADER_BUFFER_H_
#define FFLI_RESOURCE_LOADER_BUFFER_H_

#include <nn/ffl/FFLiResourceUncompressBuffer.h>

class FFLiResourceLoaderBuffer
{
public:
    FFLiResourceLoaderBuffer(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType);
    ~FFLiResourceLoaderBuffer();

    FFLiResourceUncompressBuffer& GetUncompressBuffer()
    {
        return m_ResourceUncompressBuffer;
    }

    const FFLiResourceUncompressBuffer& GetUncompressBuffer() const
    {
        return m_ResourceUncompressBuffer;
    }

private:
    FFLiResourceUncompressBuffer    m_ResourceUncompressBuffer;
    void*                           _8; // Deleted
};
NN_STATIC_ASSERT(sizeof(FFLiResourceLoaderBuffer) == 0xC);

#endif // FFLI_RESOURCE_LOADER_BUFFER_H_
