#include <nn/ffl/FFLiResourceLoaderBuffer.h>

FFLiResourceLoaderBuffer::FFLiResourceLoaderBuffer(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType)
    : m_ResourceUncompressBuffer(pResourceManager, resourceType)
{
}

FFLiResourceLoaderBuffer::~FFLiResourceLoaderBuffer()
{
}
