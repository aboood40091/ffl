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
