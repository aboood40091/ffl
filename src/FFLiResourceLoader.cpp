#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiResourceLoaderBuffer.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiUtil.h>
#include <nn/ffl/FFLiZlibInflator.h>

#include <nn/ffl/detail/FFLiBufferAllocatorForZlib.h>

#include <filedevice/rio_FileDeviceMgr.h>

#include <cstring>

namespace {

bool Uncompress(void* pDst, const void* pSrc, FFLiResourceUncompressBuffer* pBuffer, const FFLiResourcePartsInfo& partsInfo);

}

FFLiResourceLoader::FFLiResourceLoader(FFLiResourceManager* pResourceManager, FFLiResourceLoaderBuffer* pResLoaderBuffer, FFLResourceType resourceType)
    : m_pResourceManager(pResourceManager)
    , m_pBuffer(pResLoaderBuffer)
    , m_ResourceType(resourceType)
{
}

FFLiResourceLoader::~FFLiResourceLoader()
{
    Close();
}

bool FFLiResourceLoader::IsExpand() const
{
    return m_pResourceManager->IsExpand(m_ResourceType);
}

FFLiResourceHeader* FFLiResourceLoader::Header() const
{
    return m_pResourceManager->Header(m_ResourceType);
}

u32 FFLiResourceLoader::GetTextureAlignedMaxSize(FFLiTexturePartsType partsType) const
{
    return FFLiRoundUp(Header()->GetTextureMaxSize(partsType), rio::FileDevice::cBufferMinAlignment);
}

u32 FFLiResourceLoader::GetShapeAlignedMaxSize(FFLiShapePartsType partsType) const
{
    return FFLiRoundUp(Header()->GetShapeMaxSize(partsType), rio::FileDevice::cBufferMinAlignment);
}

FFLResult FFLiResourceLoader::LoadTexture(void* pData, u32* pSize, FFLiTexturePartsType partsType, u32 index)
{
    u32 num;
    FFLiResourcePartsInfo* pPartsInfo = FFLiGetTextureResoucePartsInfos(&num, Header()->GetTextureHeader(), partsType);
    if (pPartsInfo == NULL || index >= num)
        return FFL_RESULT_ERROR;

    return Load(pData, pSize, pPartsInfo[index]);
}

FFLResult FFLiResourceLoader::LoadShape(void* pData, u32* pSize, FFLiShapePartsType partsType, u32 index)
{
    u32 num;
    FFLiResourcePartsInfo* pPartsInfo = FFLiGetShapeResoucePartsInfos(&num, Header()->GetShapeHeader(), partsType);
    if (pPartsInfo == NULL || index >= num)
        return FFL_RESULT_ERROR;

    return Load(pData, pSize, pPartsInfo[index]);
}

FFLResult FFLiResourceLoader::GetPointerTextureByExpandCache(void** ppPtr, u32* pSize, FFLiTexturePartsType partsType, u32 index)
{
    if (!IsExpand())
        return FFL_RESULT_FILE_LOAD_ERROR;

    u32 num;
    FFLiResourcePartsInfo* pPartsInfo = FFLiGetTextureResoucePartsInfos(&num, Header()->GetTextureHeader(), partsType);
    if (pPartsInfo == NULL || index >= num)
        return FFL_RESULT_ERROR;

    *pSize = pPartsInfo[index].dataSize;
    return GetPointerFromCache(ppPtr, pPartsInfo[index]);
}

FFLResult FFLiResourceLoader::Load(void* pData, u32* pSize, const FFLiResourcePartsInfo& partsInfo)
{
    *pSize = partsInfo.dataSize;

    if (partsInfo.dataSize != 0)
    {
        if (IsCached())
        {
            FFLResult result = LoadFromCache(pData, partsInfo);
            if (result != FFL_RESULT_OK)
                return result;
        }
        else
        {
            FFLResult result = LoadFromFile(pData, partsInfo);
            if (result != FFL_RESULT_OK)
                return result;
        }
    }

    return FFL_RESULT_OK;
}

FFLResult FFLiResourceLoader::LoadFromCache(void* pData, const FFLiResourcePartsInfo& partsInfo)
{
    void* ptr = NULL;
    FFLResult result = GetPointerFromCache(&ptr, partsInfo);
    if (result != FFL_RESULT_OK)
        return result;

    if (partsInfo.strategy == FFLI_RESOURCE_STRATEGY_UNCOMPRESSED)
    {
        std::memcpy(pData, ptr, partsInfo.dataSize);
    }
    else
    {
        if (!Uncompress(pData, ptr, &m_pBuffer->GetUncompressBuffer(), partsInfo))
            return FFL_RESULT_FILE_LOAD_ERROR;
    }

    return FFL_RESULT_OK;
}

FFLResult FFLiResourceLoader::LoadFromFile(void* pData, const FFLiResourcePartsInfo& partsInfo)
{
    if (OpenIfClosed() != rio::RAW_ERROR_OK)
        return FFL_RESULT_RES_FS_ERROR;

    if (partsInfo.strategy == FFLI_RESOURCE_STRATEGY_UNCOMPRESSED)
    {
        if (ReadWithPos(pData, partsInfo.dataPos, partsInfo.dataSize) != 1)
            return FFL_RESULT_RES_FS_ERROR;
    }
    else
    {
        if (ReadWithPos(m_pBuffer->GetUncompressBuffer().Buffer(), partsInfo.dataPos, partsInfo.compressedSize) != 1)
            return FFL_RESULT_RES_FS_ERROR;

        if (!Uncompress(pData, m_pBuffer->GetUncompressBuffer().Buffer(), &m_pBuffer->GetUncompressBuffer(), partsInfo))
            return FFL_RESULT_FILE_LOAD_ERROR;
    }

    return FFL_RESULT_OK;
}

bool FFLiResourceLoader::IsCached() const
{
    return m_pResourceManager->IsCached();
}

FFLResult FFLiResourceLoader::GetPointerFromCache(void** ppPtr, const FFLiResourcePartsInfo& partsInfo)
{
    if (!IsCached())
        return FFL_RESULT_FILE_LOAD_ERROR;

    *ppPtr = (u8*)m_pResourceManager->GetResourceCache().Header(m_ResourceType) + partsInfo.dataPos;
    return FFL_RESULT_OK;
}

rio::RawErrorCode FFLiResourceLoader::OpenIfClosed()
{
    if (m_FileHandle.getDevice() != nullptr)
        return rio::RAW_ERROR_OK;

    rio::NativeFileDevice* device = rio::FileDeviceMgr::instance()->getNativeFileDevice();
    if (!device->tryOpen(&m_FileHandle, m_pResourceManager->GetPath(m_ResourceType), rio::FileDevice::FILE_OPEN_FLAG_READ))
    {
        rio::RawErrorCode status = m_FileHandle.getDevice()->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return status;
    }

    return rio::RAW_ERROR_OK;
}

rio::RawErrorCode FFLiResourceLoader::ReadWithPos(void* pDst, u32 pos, u32 size)
{
    if (!m_FileHandle.trySeek(pos, rio::FileDevice::SEEK_ORIGIN_BEGIN))
    {
        rio::RawErrorCode status = m_FileHandle.getDevice()->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return status;
    }

    u32 readSize = 0;
    if (!m_FileHandle.tryRead(&readSize, (u8*)pDst, FFLiRoundUp(size, rio::FileDevice::cBufferMinAlignment)))
    {
        rio::RawErrorCode status = m_FileHandle.getDevice()->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return status;
    }

    return rio::RAW_ERROR_OK;
}

rio::RawErrorCode FFLiResourceLoader::Close()
{
    if (m_FileHandle.getDevice() == nullptr)
        return rio::RAW_ERROR_OK;

    if (!m_FileHandle.tryClose())
    {
        rio::RawErrorCode status = m_FileHandle.getDevice()->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return status;
    }

    return rio::RAW_ERROR_OK;
}

namespace {

bool Uncompress(void* pDst, const void* pSrc, FFLiResourceUncompressBuffer* pBuffer, const FFLiResourcePartsInfo& partsInfo)
{
    FFLiBufferAllocatorForZlib allocator(pBuffer->TempBuffer(), pBuffer->TempBufferSize());
    FFLiZlibInflator inflator(allocator, FFLiResourceWindowBitsToZlibWindowBits(FFLiResourceWindowBits(partsInfo.windowBits)));

    void* dst = pDst;
    u32 dstSize = partsInfo.dataSize;

    const void* src = pSrc;
    u32 srcSize = partsInfo.compressedSize;

    return inflator.Process(&dst, &dstSize, &src, &srcSize, Z_FINISH) == Z_STREAM_END;
}

}
