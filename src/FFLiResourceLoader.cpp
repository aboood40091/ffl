#include <nn/ffl/FFLiFsCommand.h>
#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiResourceLoaderBuffer.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiUtil.h>
#include <nn/ffl/FFLiZlibInflator.h>

#include <nn/ffl/detail/FFLiBufferAllocatorForZlib.h>

#include <cstring>

namespace {

bool Uncompress(void* pDst, const void* pSrc, FFLiResourceUncompressBuffer* pBuffer, const FFLiResourcePartsInfo& partsInfo);

}

FFLiResourceLoader::FFLiResourceLoader(FFLiResourceManager* pResourceManager, FFLiResourceLoaderBuffer* pResLoaderBuffer, FFLResourceType resourceType)
    : m_pResourceManager(pResourceManager)
    , m_pBuffer(pResLoaderBuffer)
    , m_pFsCommand(FFLiFsCommand::PlacementNew(pResLoaderBuffer->GetCommandBuffer(), pResourceManager->GetClient()))
    , m_FsFile(m_pFsCommand)
    , m_ResourceType(resourceType)
{
}

FFLiResourceLoader::~FFLiResourceLoader()
{
    Close();

    FFLiFsCommand::PlacementDelete(m_pFsCommand);
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
    return FFLiRoundUp(Header()->GetTextureMaxSize(partsType), FS_IO_BUFFER_ALIGN);
}

u32 FFLiResourceLoader::GetShapeAlignedMaxSize(FFLiShapePartsType partsType) const
{
    return FFLiRoundUp(Header()->GetShapeMaxSize(partsType), FS_IO_BUFFER_ALIGN);
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
    if (OpenIfClosed() != FS_STATUS_OK)
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

FSStatus FFLiResourceLoader::OpenIfClosed()
{
    if (m_FsFile.IsOpened())
        return FS_STATUS_OK;

    return m_FsFile.Open(m_pResourceManager->GetPath(m_ResourceType), "r");
}

FSStatus FFLiResourceLoader::ReadWithPos(void* pDst, u32 pos, u32 size)
{
    return m_FsFile.ReadWithPos(pDst, FFLiRoundUp(size, FS_IO_BUFFER_ALIGN), 1, pos);
}

FSStatus FFLiResourceLoader::Close()
{
    if (!m_FsFile.IsOpened())
        return FS_STATUS_OK;

    return m_FsFile.Close();
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
