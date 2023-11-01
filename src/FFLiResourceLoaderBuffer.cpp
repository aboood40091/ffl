#include <nn/ffl/FFLiResourceLoaderBuffer.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

FFLiResourceLoaderBuffer::FFLiResourceLoaderBuffer(const FFLiResourceManager* pResourceManager, FFLiBufferAllocator* pAllocator, FFLResourceType resourceType)
    : m_ResourceUncompressBuffer(pResourceManager, pAllocator, resourceType)
{
}

FFLiResourceLoaderBuffer::~FFLiResourceLoaderBuffer()
{
}
