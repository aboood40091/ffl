#include <nn/ffl/FFLCharModelDesc.h>

#include <nn/ffl/FFLiCharModelCreateParam.h>
#include <nn/ffl/FFLiFacelineTexture.h>
#include <nn/ffl/FFLiMaskTextures.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiResourceUncompressBuffer.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiTextureTempObject.h>
#include <nn/ffl/FFLiUtil.h>

FFLiCharModelCreateParam::FFLiCharModelCreateParam(FFLiDatabaseManager* pDatabaseManager, FFLiResourceManager* pResourceManager, FFLiShaderCallback* pCallback)
    : m_pDatabaseManager(pDatabaseManager)
    , m_pResourceManager(pResourceManager)
    , m_pShaderCallback(pCallback)
{
}

FFLiCharModelCreateParam::~FFLiCharModelCreateParam()
{
}

u32 FFLiCharModelCreateParam::GetResolution(FFLResolution resolution)
{
    return resolution & FFL_RESOLUTION_MASK;
}

bool FFLiCharModelCreateParam::IsEnabledMipMap(FFLResolution resolution)
{
    return resolution & FFL_RESOLUTION_MIP_MAP_ENABLE_MASK;
}

bool FFLiCharModelCreateParam::CheckModelDesc(const FFLCharModelDesc* pDesc)
{
    if (pDesc == NULL)
        return false;

    if (pDesc->resourceType >= FFL_RESOURCE_TYPE_MAX)
        return false;

    if ((pDesc->expressionFlag & 0x7ffff) == 0)
        return false;

    if ((pDesc->modelFlag & 7) == 0)
        return false;

    return true;
}

u32 FFLiCharModelCreateParam::GetBufferSize(const FFLCharModelDesc* pDesc) const
{
    if (!CheckModelDesc(pDesc))
        return 0;

    FFLResolution _resolution = pDesc->resolution;
    u32 resolution = GetResolution(_resolution);
    bool isEnabledMipMap = IsEnabledMipMap(_resolution);

    FFLResourceType resourceType = pDesc->resourceType;
    u32 bufferSize = 0;

    if (!m_pResourceManager->IsValid(resourceType))
        return 0;

    for (u32 shapePartsType = 0; shapePartsType < FFLI_SHAPE_PARTS_TYPE_MAX; ++shapePartsType)
        bufferSize += m_pResourceManager->GetShapeAlignedMaxSize(resourceType, FFLiShapePartsType(shapePartsType));

    bufferSize += FFLiGetBufferSizeFacelineTexture(resolution, isEnabledMipMap);
    bufferSize += FFLiGetBufferSizeMaskTextures(pDesc->expressionFlag, resolution, isEnabledMipMap);
    bufferSize += FFLiGetTextureMaxSizeWithAlign(m_pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_CAP);
    bufferSize += FFLiGetTextureMaxSizeWithAlign(m_pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_NOSELINE);
    bufferSize += FFLiGetTextureMaxSizeWithAlign(m_pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_GLASS);

    return bufferSize;
}

u32 FFLiCharModelCreateParam::GetTempBufferSize(const FFLCharModelDesc* pDesc) const
{
    if (!CheckModelDesc(pDesc))
        return 0;

    FFLResolution _resolution = pDesc->resolution;
    u32 resolution = GetResolution(_resolution);
    bool isEnabledMipMap = IsEnabledMipMap(_resolution);

    FFLResourceType resourceType = pDesc->resourceType;

    if (!m_pResourceManager->IsValid(resourceType))
        return 0;

    u32 bufferSize = FFLiGetTempBufferSizeMaskTextures(pDesc->expressionFlag, resolution, isEnabledMipMap, m_pResourceManager, resourceType);
    bufferSize += sizeof(FFLiTextureTempObject);
    bufferSize += FFLiGetTempBufferSizeFacelineTexture(resolution, isEnabledMipMap, m_pResourceManager, resourceType);
    bufferSize += FFLiResourceUncompressBuffer::GetBufferSize(m_pResourceManager, resourceType);

    return bufferSize;
}
