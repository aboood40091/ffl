#ifndef FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_
#define FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_

#include <nn/ffl/FFLResourceType.h>

class FFLiResourceManager;

class FFLiResourceUncompressBuffer
{
public:
    static u32 GetBufferSize(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType);
};

#endif // FFLI_RESOURCE_UNCOMPRESS_BUFFER_H_
