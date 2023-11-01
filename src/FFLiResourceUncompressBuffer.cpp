#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiResourceUncompressBuffer.h>

#include <filedevice/rio_FileDevice.h>

FFLiResourceUncompressBuffer::FFLiResourceUncompressBuffer(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType)
    : m_pBuffer(rio::MemUtil::alloc(pResourceManager->GetUncompressBufferSize(resourceType), rio::FileDevice::cBufferMinAlignment))
{
}

FFLiResourceUncompressBuffer::~FFLiResourceUncompressBuffer()
{
    if (m_pBuffer != nullptr)
    {
        rio::MemUtil::free(m_pBuffer);
        m_pBuffer = nullptr;
    }
}

void* FFLiResourceUncompressBuffer::Buffer()
{
    return m_pBuffer;
}
