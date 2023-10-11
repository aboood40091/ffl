#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiResourceUncompressBuffer.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

#include <cafe/fs.h>

static u32 GetTempBufferSize()
{
    return 0x19000; // 100 KB
}

FFLiResourceUncompressBuffer::FFLiResourceUncompressBuffer(const FFLiResourceManager* pResourceManager, FFLiBufferAllocator* pAllocator, FFLResourceType resourceType)
    : m_pBuffer(pAllocator->Allocate(pResourceManager->GetUncompressBufferSize(resourceType), FS_IO_BUFFER_ALIGN))
    , m_pTempBuffer(pAllocator->Allocate(GetTempBufferSize()))
{
}

FFLiResourceUncompressBuffer::~FFLiResourceUncompressBuffer()
{
}

void* FFLiResourceUncompressBuffer::Buffer()
{
    return m_pBuffer;
}

void* FFLiResourceUncompressBuffer::TempBuffer()
{
    return m_pTempBuffer;
}

u32 FFLiResourceUncompressBuffer::TempBufferSize() const
{
    return GetTempBufferSize();
}

u32 FFLiResourceUncompressBuffer::GetBufferSize(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType)
{
    u32 ret  = FS_IO_BUFFER_ALIGN;
    ret     += GetTempBufferSize();
    ret     += pResourceManager->GetUncompressBufferSize(resourceType);

    return ret;
}
