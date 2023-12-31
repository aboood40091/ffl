#include <nn/ffl/FFLiFsCommand.h>
#include <nn/ffl/FFliFsFile.h>
#include <nn/ffl/FFLiPath.h>
#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiResourceUtil.h>
#include <nn/ffl/FFLiUtil.h>

#include <cstring>

namespace {

const char* RESOUCE_FILE_NAME[2][FFL_RESOURCE_TYPE_MAX] = {
    {
        "FFLResMiddle.dat",
        "FFLResHigh.dat"
    },
    {
        "FFLResMiddleLG.dat",
        "FFLResHighLG.dat"
    }
};

}

FFLiResourceManager::FFLiResourceManager(FFLiResourceMultiHeader* pHeader, FFLiFsClient* pClient)
    : m_pResourceMultiHeader(pHeader)
    , m_pFsClient(pClient)
{
    std::memset(m_pResourceMultiHeader, 0, sizeof(FFLiResourceMultiHeader));
    std::memset(m_Path, 0, FFL_RESOURCE_TYPE_MAX * FFL_PATH_MAX_LEN);
}

FFLiResourceManager::~FFLiResourceManager()
{
}

const char* FFLiResourceManager::GetRelativeResourcePath(FFLResourceType resourceType, bool LG)
{
    return RESOUCE_FILE_NAME[LG][resourceType];
}

FFLResult FFLiResourceManager::GetResourcePath(char* pDst, u32 size, FFLResourceType resourceType, bool LG)
{
    FSStatus status = FFLiGetResourcePath(pDst, size, GetRelativeResourcePath(resourceType, LG));
    return FFLiConvertFSStatusToFFLResult(status);
}

FFLResult FFLiResourceManager::AfterConstruct()
{
    for (u32 i = 0; i < FFL_RESOURCE_TYPE_MAX; i++)
    {
        FFLResult result = GetResourcePath(m_Path[i], FFL_PATH_MAX_LEN, FFLResourceType(i), false);
        if (result != FFL_RESULT_OK)
            return result;
    }
    return FFL_RESULT_OK;
}

FFLResult FFLiResourceManager::LoadResourceHeader(FFLiFsCommandBuffer* pCommandBuffer)
{
    FFLiFsCommand* pCommand = FFLiFsCommand::PlacementNew(pCommandBuffer, m_pFsClient);
    FFLResult result = LoadResourceHeaderImpl(pCommand);
    FFLiFsCommand::PlacementDelete(pCommand);
    return result;
}

FFLResult FFLiResourceManager::LoadResourceHeaderImpl(FFLiFsCommand* pCommand)
{
    FFLiFsFile file(pCommand);

    for (u32 i = 0; i < FFL_RESOURCE_TYPE_MAX; i++)
    {
        FFLiResourceHeader* pHeader = &(m_pResourceMultiHeader->header[i]);

        if (file.Open(GetPath(FFLResourceType(i)), "r") != FS_STATUS_OK)
            return FFL_RESULT_FILE_INVALID;

        if (file.Read(pHeader, sizeof(FFLiResourceHeader), 1) != 1)
        {
            file.Close();
            return FFL_RESULT_FILE_INVALID;
        }

        if (file.Close() != FS_STATUS_OK)
            return FFL_RESULT_FILE_INVALID;

        FFLResult result = FFLiIsVaildResourceHeader(pHeader);
        if (result != FFL_RESULT_OK)
            return result;
    }

    return FFL_RESULT_OK;
}

FFLResult FFLiResourceManager::AttachCache(const void* pData, u32 size, FFLResourceType resourceType)
{
    return m_ResourceCache.Attach(pData, size, resourceType);
}

bool FFLiResourceManager::IsCached() const
{
    return m_ResourceCache.IsAttached();
}

FFLiResourceHeader* FFLiResourceManager::Header(FFLResourceType resourceType) const
{
    if (IsCached())
        return HeaderFromCache(resourceType);

    else
        return HeaderFromFile(resourceType);
}

FFLiResourceHeader* FFLiResourceManager::HeaderFromCache(FFLResourceType resourceType) const
{
    return m_ResourceCache.Header(resourceType);
}

FFLiResourceHeader* FFLiResourceManager::HeaderFromFile(FFLResourceType resourceType) const
{
    return &(m_pResourceMultiHeader->header[resourceType]);
}

u32 FFLiResourceManager::GetTextureAlignedMaxSize(FFLResourceType resourceType, FFLiTexturePartsType partsType) const
{
    return FFLiRoundUp(Header(resourceType)->GetTextureMaxSize(partsType), FS_IO_BUFFER_ALIGN);
}

u32 FFLiResourceManager::GetShapeAlignedMaxSize(FFLResourceType resourceType, FFLiShapePartsType partsType) const
{
    return FFLiRoundUp(Header(resourceType)->GetShapeMaxSize(partsType), FS_IO_BUFFER_ALIGN);
}

bool FFLiResourceManager::IsValid(FFLResourceType resourceType) const
{
    if (!IsCached())
        return true;

    return m_ResourceCache.IsValid(resourceType);
}

bool FFLiResourceManager::IsExpand(FFLResourceType resourceType) const
{
    if (!IsValid(resourceType) || !IsCached())
        return false;

    return Header(resourceType)->IsExpand();
}

u32 FFLiResourceManager::GetUncompressBufferSize(FFLResourceType resourceType) const
{
    return FFLiRoundUp(Header(resourceType)->GetUncompressBufferSize() + FS_IO_BUFFER_ALIGN, FS_IO_BUFFER_ALIGN);
}

const char* FFLiResourceManager::GetPath(FFLResourceType resourceType) const
{
    return m_Path[resourceType];
}
