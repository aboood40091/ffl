#include <nn/ffl/FFLiFsCommand.h>
#include <nn/ffl/FFLiResourceLoaderBuffer.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

FFLiResourceLoaderBuffer::FFLiResourceLoaderBuffer(const FFLiResourceManager* pResourceManager, FFLiBufferAllocator* pAllocator, FFLResourceType resourceType)
    : m_ResourceUncompressBuffer(pResourceManager, pAllocator, resourceType)
    , m_pFsCommandBuffer(static_cast<FFLiFsCommandBuffer*>(pAllocator->Allocate(sizeof(FFLiFsCommandBuffer))))
{
}

FFLiResourceLoaderBuffer::~FFLiResourceLoaderBuffer()
{
}
