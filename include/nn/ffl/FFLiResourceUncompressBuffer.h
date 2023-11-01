#ifndef FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_
#define FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_

#include <nn/ffl/FFLResourceType.h>

class FFLiResourceManager;

class FFLiResourceUncompressBuffer
{
public:
    FFLiResourceUncompressBuffer(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType);
    ~FFLiResourceUncompressBuffer();

    void* Buffer();

private:
    void*   m_pBuffer;
    void*   _4; // Deleted
};
NN_STATIC_ASSERT(sizeof(FFLiResourceUncompressBuffer) == 8);

#endif // FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_
