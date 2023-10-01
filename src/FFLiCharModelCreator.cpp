#include <nn/ffl/FFLBoundingBox.h>
#include <nn/ffl/FFLCharModelBuffer.h>
#include <nn/ffl/FFLCharModelDesc.h>
#include <nn/ffl/FFLCharModelSource.h>
#include <nn/ffl/FFLModelType.h>

#include <nn/ffl/FFLiCharModel.h>
#include <nn/ffl/FFLiCharModelCreateParam.h>
#include <nn/ffl/FFLiCharModelCreator.h>
#include <nn/ffl/FFLiCompressor.h>
#include <nn/ffl/FFLiFacelineTexture.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiResourceLoaderBuffer.h>
#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiModulate.h>
#include <nn/ffl/FFLiRenderTextureBuffer.h>
#include <nn/ffl/FFLiShape.h>
#include <nn/ffl/FFLiShapePartsType.h>
#include <nn/ffl/FFLiShapeType.h>
#include <nn/ffl/FFLiTemp.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiTextureTempObject.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

#include <cstring>

class FFLiResourceLoader;

union F32BitCast
{
    f32 f;
    u32 u;
    struct
    {
        u32 mantissa    : 23;
        u32 exponent    : 8;
        u32 sign        : 1;
    };
};
NN_STATIC_ASSERT(sizeof(F32BitCast) == 4);

static bool IsNaN(f32 value)
{
    F32BitCast x = { value };
    // Basically:
    // return x.exponent == 0xff && x.mantissa > 0;
    return (x.u << 1) > 0xff000000;
}

FFLiCharModelCreator::FFLiCharModelCreator(FFLiCharModelCreateParam* pParam, FFLiManager* pManager)
    : m_pCharModelCreateParam(pParam)
    , m_pManager(pManager)
{
}

FFLiCharModelCreator::~FFLiCharModelCreator()
{
}

namespace {

FFLModelType ModelFlagToModelType(u32 flag);
FFLResult InitShapes(FFLiCharModel* pModel, FFLiResourceLoader * pResLoader, FFLiBufferAllocator* pAllocator, const FFLiCoordinate* pCoordinate);
FFLResult InitTextures(FFLiCharModel* pModel, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator);
void AdjustPartsTransform(FFLiCharModel* pModel, const FFLiCoordinate* pCoordinate);
void SetupDrawParam(FFLiCharModel* pModel);

}

FFLResult FFLiCharModelCreator::ExecuteCPUStep(FFLiCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc, FFLCharModelBuffer* pBuffer)
{
    if (!FFLiCharModelCreateParam::CheckModelDesc(pDesc, m_pManager->GetCompressor() != NULL))
        return FFL_RESULT_ERROR;

    std::memset(pModel, 0, sizeof(FFLiCharModel));
    
    pModel->charModelDesc = *pDesc;
    pModel->pBuffer = pBuffer->pBuffer;
    pModel->bufferSize = pBuffer->bufferSize;
    pModel->modelType = ModelFlagToModelType(pModel->charModelDesc.modelFlag);

    FFLResult result = m_pCharModelCreateParam->GetDatabaseManager()->PickupCharInfo(&pModel->charInfo, pSource->dataSource, pSource->pBuffer, pSource->index);
    if (result != FFL_RESULT_OK)
        return result;
    
    u32 resolution = FFLiCharModelCreateParam::GetResolution(pDesc->resolution);
    bool isEnabledMipMap = FFLiCharModelCreateParam::IsEnabledMipMap(pDesc->resolution);

    FFLiBufferAllocator tempAllocator;
    FFLiBufferAllocator allocator;

    allocator.Init(pBuffer->pBuffer, pBuffer->bufferSize);
    tempAllocator.Init(pBuffer->pTempBuffer, pBuffer->tempBufferSize);

    FFLResourceType resourceType = pDesc->resourceType;

    if (!m_pCharModelCreateParam->GetResourceManager()->IsValid(resourceType))
        return FFL_RESULT_ERROR;

    FFLiResourceLoaderBuffer resLoaderBuffer(m_pCharModelCreateParam->GetResourceManager(), &tempAllocator, resourceType);
    FFLiResourceLoader resLoader(m_pCharModelCreateParam->GetResourceManager(), &resLoaderBuffer, resourceType);

    pModel->pTextureTempObject = static_cast<FFLiTextureTempObject*>(tempAllocator.Allocate(sizeof(FFLiTextureTempObject), 4));

    FFLiRenderTextureBuffer renderTextureBuffer;
    SetupRenderTextureBuffer(renderTextureBuffer, pDesc, &tempAllocator);

    pModel->expression = FFLiInitMaskTextures(&pModel->maskTextures, pDesc->expressionFlag, resolution, isEnabledMipMap, pDesc->enableCompressorParam, &allocator);

    bool useCompressorUB = false;
    if (m_pManager->GetCompressor() != NULL)
        useCompressorUB = m_pManager->GetCompressor()->UseUB();

    result = FFLiInitTempObjectMaskTextures(&pModel->pTextureTempObject->maskTextures, &pModel->maskTextures, &pModel->charInfo, pDesc->expressionFlag, resolution, isEnabledMipMap, pDesc->enableCompressorParam, useCompressorUB, &resLoader, &tempAllocator, &renderTextureBuffer);
    if (result != FFL_RESULT_OK)
        return result;
    
    FFLiInitFacelineTexture(&pModel->facelineRenderTexture, resolution, isEnabledMipMap, pDesc->enableCompressorParam, &allocator);

    result = FFLiInitTempObjectFacelineTexture(&pModel->pTextureTempObject->facelineTexture, &pModel->facelineRenderTexture, &pModel->charInfo, resolution, isEnabledMipMap, pDesc->enableCompressorParam, useCompressorUB, &resLoader, &tempAllocator, &renderTextureBuffer);
    if (result != FFL_RESULT_OK)
        return result;
    
    result = InitShapes(pModel, &resLoader, &allocator, &m_pCharModelCreateParam->GetCoordinate());
    if (result != FFL_RESULT_OK)
        return result;
    
    result = InitTextures(pModel, &resLoader, &allocator);
    if (result != FFL_RESULT_OK)
        return result;
    
    AdjustPartsTransform(pModel, &m_pCharModelCreateParam->GetCoordinate());
    SetupDrawParam(pModel);
    
    return FFL_RESULT_OK;
}

void FFLiCharModelCreator::ExecuteGPUStep(FFLiCharModel* pModel, const FFLShaderCallback* pCallback)
{
    u32 resolution = FFLiCharModelCreateParam::GetResolution(pModel->charModelDesc.resolution);
    bool isEnabledMipMap = FFLiCharModelCreateParam::IsEnabledMipMap(pModel->charModelDesc.resolution);

    FFLiShaderCallback shaderCallback;
    shaderCallback.Set(pCallback);
    
    shaderCallback.CallSetContextState();

    Mat44 mat;
    MAT44Identity(&mat);
    shaderCallback.CallSetMatrix(mat);

    FFLiRenderMaskTextures(&pModel->maskTextures, &pModel->pTextureTempObject->maskTextures, &shaderCallback, &m_pManager->GetCopySurface(), &m_pManager->GetCompressor()->GetCompressorImplBC3());
    FFLiRenderFacelineTexture(&pModel->facelineRenderTexture, &pModel->charInfo, resolution, &pModel->pTextureTempObject->facelineTexture, &shaderCallback, &m_pManager->GetCopySurface(), &m_pManager->GetCompressor()->GetCompressorImplBC1());

    AfterExecuteGPUStep(pModel);

    pModel->pTextureTempObject = NULL;
}

namespace {

FFLModelType ModelFlagToModelType(u32 flag)
{
    for (u32 i = 0; i < FFL_MODEL_TYPE_MAX; ++i)
        if (flag & 1 << (i & 0x3f))
            return FFLModelType(i);
    
    return FFL_MODEL_TYPE_0;
}

FFLiShapeType ConvertShapePartsTypeToShapeType(FFLiShapePartsType partsType)
{
    switch (partsType)
    {
    case FFLI_SHAPE_PARTS_TYPE_BEARD:
        return FFLI_SHAPE_TYPE_OPA_BEARD;
    case FFLI_SHAPE_PARTS_TYPE_CAP_1:
        return FFLI_SHAPE_TYPE_OPA_CAP_1;
    case FFLI_SHAPE_PARTS_TYPE_CAP_2:
        return FFLI_SHAPE_TYPE_OPA_CAP_2;
    case FFLI_SHAPE_PARTS_TYPE_FACELINE:
        return FFLI_SHAPE_TYPE_OPA_FACELINE;
    case FFLI_SHAPE_PARTS_TYPE_GLASS:
        return FFLI_SHAPE_TYPE_XLU_GLASS;
    case FFLI_SHAPE_PARTS_TYPE_MASK:
        return FFLI_SHAPE_TYPE_XLU_MASK;
    case FFLI_SHAPE_PARTS_TYPE_NOSELINE:
        return FFLI_SHAPE_TYPE_XLU_NOSELINE;
    case FFLI_SHAPE_PARTS_TYPE_NOSE:
        return FFLI_SHAPE_TYPE_OPA_NOSE;
    case FFLI_SHAPE_PARTS_TYPE_HAIR_1:
        return FFLI_SHAPE_TYPE_OPA_HAIR_1;
    case FFLI_SHAPE_PARTS_TYPE_HAIR_2:
        return FFLI_SHAPE_TYPE_OPA_HAIR_2;
    case FFLI_SHAPE_PARTS_TYPE_FOREHEAD_1:
        return FFLI_SHAPE_TYPE_OPA_FOREHEAD_1;
    case FFLI_SHAPE_PARTS_TYPE_FOREHEAD_2:
        return FFLI_SHAPE_TYPE_OPA_FOREHEAD_2;
    }

    return FFLI_SHAPE_TYPE_MAX;
}

void UpdateBoundingBox(FFLBoundingBox* pDst, const FFLBoundingBox* pSrc)
{
    if (!IsNaN(pSrc->min.x))
    {
        if (IsNaN(pDst->min.x))
        {
            *pDst = *pSrc;
            return;
        }

        pDst->min.x = FFLiMin(pDst->min.x, pSrc->min.x);
        pDst->min.y = FFLiMin(pDst->min.y, pSrc->min.y);
        pDst->min.z = FFLiMin(pDst->min.z, pSrc->min.z);

        pDst->max.x = FFLiMax(pDst->max.x, pSrc->max.x);
        pDst->max.y = FFLiMax(pDst->max.y, pSrc->max.y);
        pDst->max.z = FFLiMax(pDst->max.z, pSrc->max.z);
    }
}

static const bool s_UpdateBoundingBox[FFLI_SHAPE_PARTS_TYPE_MAX][3] = {
    {  true,  true,  true },
    {  true, false, false },
    { false,  true, false },
    {  true,  true,  true },
    { false, false, false },
    { false, false, false },
    {  true,  true,  true },
    {  true,  true,  true },
    {  true, false, false },
    { false,  true, false },
    {  true, false, false },
    { false,  true, false }
};

void CalcluateBoundingBox(FFLBoundingBox* pDst, const FFLBoundingBox* pSrc, FFLiShapePartsType partsType)
{
    for (u32 i = 0; i < 3; i++)
        if (s_UpdateBoundingBox[partsType][i])
            UpdateBoundingBox(&(pDst[i]), pSrc);
}

FFLResult InitShape(FFLiCharModel* pModel, FFLiShapePartsType partsType, u32 index, f32 scaleX, f32 scaleY, const FFLVec3* pTranslate, bool flipX, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator, const FFLiCoordinate* pCoordinate)
{
    FFLiShapeType type = ConvertShapePartsTypeToShapeType(partsType);
    FFLDrawParam* pParam = &(pModel->drawParam[type]);
    FFLBoundingBox boundingBox;
    
    FFLResult result = FFLiLoadShape(pParam, &boundingBox, pModel, partsType, index, pResLoader, pAllocator);
    if (result != FFL_RESULT_OK)
        return result;

    FFLiAdjustShape(pParam, &boundingBox, scaleX, scaleY, pTranslate, flipX, pCoordinate, partsType, pModel->charModelDesc.modelFlag >> 3 & 1);
    CalcluateBoundingBox(pModel->boundingBox, &boundingBox, partsType);
    return FFL_RESULT_OK;
}

FFLResult InitShapes(FFLiCharModel* pModel, FFLiResourceLoader * pResLoader, FFLiBufferAllocator* pAllocator, const FFLiCoordinate* pCoordinate)
{
    u32 modelFlag = pModel->charModelDesc.modelFlag & 7;

    FFLResult result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_FACELINE, pModel->charInfo.parts.faceType, 1.0f, 1.0f, NULL, false, pResLoader, pAllocator, pCoordinate);
    if (result != FFL_RESULT_OK)
        return result;

    if (modelFlag & (1 << FFL_MODEL_TYPE_0 |
                     1 << FFL_MODEL_TYPE_1))
    {
        bool modelType0Enable = modelFlag & 1 << FFL_MODEL_TYPE_0;
        bool modelType1Enable = modelFlag & 1 << FFL_MODEL_TYPE_1;
        
        bool flipHair = pModel->charInfo.parts.hairDir > 0;

        struct
        {
            bool                enable;
            FFLiShapePartsType  partsType;
        } modelTypeShapePartsInfo[2 * 3] = {
            // FFL_MODEL_TYPE_0
            { false, FFLI_SHAPE_PARTS_TYPE_HAIR_1 },
            { false, FFLI_SHAPE_PARTS_TYPE_CAP_1 },
            { false, FFLI_SHAPE_PARTS_TYPE_FOREHEAD_1 },
            // FFL_MODEL_TYPE_1
            { false, FFLI_SHAPE_PARTS_TYPE_HAIR_2 },
            { false, FFLI_SHAPE_PARTS_TYPE_CAP_2 },
            { false, FFLI_SHAPE_PARTS_TYPE_FOREHEAD_2 }
        };

        modelTypeShapePartsInfo[0 * 3 + 0].enable = modelType0Enable;
        modelTypeShapePartsInfo[0 * 3 + 1].enable = modelType0Enable;
        modelTypeShapePartsInfo[0 * 3 + 2].enable = modelType0Enable;

        modelTypeShapePartsInfo[1 * 3 + 0].enable = modelType1Enable;
        modelTypeShapePartsInfo[1 * 3 + 1].enable = modelType1Enable;
        modelTypeShapePartsInfo[1 * 3 + 2].enable = modelType1Enable;

        for (u32 i = 0; i < 2 * 3; i++)
        {
            if (modelTypeShapePartsInfo[i].enable)
            {
                result = InitShape(pModel, modelTypeShapePartsInfo[i].partsType, pModel->charInfo.parts.hairType, 1.0f, 1.0f, &pModel->hairPos, flipHair, pResLoader, pAllocator, pCoordinate);
                if (result != FFL_RESULT_OK)
                    return result;
            }
        }

    }

    if (pModel->charInfo.parts.beardType < 4)
    {
        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_BEARD, pModel->charInfo.parts.beardType, 1.0f, 1.0f, &pModel->beardPos, false, pResLoader, pAllocator, pCoordinate);
        if (result != FFL_RESULT_OK)
            return result;
    }

    {
        f32 noseScale = pModel->charInfo.parts.noseScale * 0.175f + 0.4f;

        FFLVec3 nosePos;
        nosePos.x = pModel->faceCenterPos.x;
        nosePos.y = pModel->faceCenterPos.y + (pModel->charInfo.parts.nosePositionY - 8) * -1.5f;
        nosePos.z = pModel->faceCenterPos.z;

        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_NOSE, pModel->charInfo.parts.noseType, noseScale, noseScale, &nosePos, false, pResLoader, pAllocator, pCoordinate);
        if (result != FFL_RESULT_OK)
            return result;

        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_NOSELINE, pModel->charInfo.parts.noseType, noseScale, noseScale, &nosePos, false, pResLoader, pAllocator, pCoordinate);
        if (result != FFL_RESULT_OK)
            return result;
    }

    result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_MASK, pModel->charInfo.parts.faceType, 1.0f, 1.0f, NULL, false, pResLoader, pAllocator, pCoordinate);
    if (result != FFL_RESULT_OK)
        return result;

    if (pModel->charInfo.parts.glassType > 0)
    {
        f32 glassScale = pModel->charInfo.parts.glassScale * 0.15f + 0.4f;

        FFLVec3 glassPos;
        glassPos.x = pModel->faceCenterPos.x;
        glassPos.y = pModel->faceCenterPos.y + (pModel->charInfo.parts.glassPositionY - 11) * -1.5f + 5.0f;
        glassPos.z = pModel->faceCenterPos.z + 2.0f;

        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_GLASS, 0, glassScale, glassScale, &glassPos, false, pResLoader, pAllocator, pCoordinate);
        if (result != FFL_RESULT_OK)
            return result;
    }
    
    return FFL_RESULT_OK;
}

FFLResult InitTextures(FFLiCharModel* pModel, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator)
{
    FFLResult result = FFL_RESULT_OK;
    
    if (FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_CAP_1])) ||
        FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_CAP_2])))
    {
        result = FFLiLoadTextureWithAllocate(&pModel->pCapTexture, FFLI_TEXTURE_PARTS_TYPE_CAP, pModel->charInfo.parts.hairType, pResLoader, pAllocator);
        if (result != FFL_RESULT_OK)
            return result;
    }
    
    if (FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE])))
    {
        result = FFLiLoadTextureWithAllocate(&pModel->pNoselineTexture, FFLI_TEXTURE_PARTS_TYPE_NOSELINE, pModel->charInfo.parts.noseType, pResLoader, pAllocator);
        if (result != FFL_RESULT_OK)
            return result;
    }
    
    if (pModel->charInfo.parts.glassType > 0)
    {
        result = FFLiLoadTextureWithAllocate(&pModel->pGlassTexture, FFLI_TEXTURE_PARTS_TYPE_GLASS, pModel->charInfo.parts.glassType, pResLoader, pAllocator);
        if (result != FFL_RESULT_OK)
            return result;
    }
    
    return result;
}

void AddVec3(FFLVec3* pDst, FFLVec3 vec)
{
    pDst->x += vec.x;
    pDst->y += vec.y;
    pDst->z += vec.z;
}

void AdjustPartsTransform(FFLiCharModel* pModel, const FFLiCoordinate* pCoordinate)
{
    AddVec3(&pModel->partsTransform._18, pModel->hairPos);
    AddVec3(&pModel->partsTransform._30, pModel->hairPos);
    AddVec3(&pModel->partsTransform._48, pModel->hairPos);

    pCoordinate->Transform              (&pModel->partsTransform._0);
    pCoordinate->TransformWithoutScale  (&pModel->partsTransform._c);
    pCoordinate->Transform              (&pModel->partsTransform._18);
    pCoordinate->TransformWithoutScale  (&pModel->partsTransform._24);
    pCoordinate->Transform              (&pModel->partsTransform._30);
    pCoordinate->TransformWithoutScale  (&pModel->partsTransform._3c);
    pCoordinate->Transform              (&pModel->partsTransform._48);
}

struct FFLiShapeTypeInfo
{
    FFLiShapeType   hairIndex;
    FFLiShapeType   foreheadIndex;
    FFLiShapeType   capIndex;
};

static const FFLiShapeTypeInfo s_ShapeTypeInfo_0 = {
    FFLI_SHAPE_TYPE_OPA_HAIR_1,
    FFLI_SHAPE_TYPE_OPA_FOREHEAD_1,
    FFLI_SHAPE_TYPE_OPA_CAP_1
};

static const FFLiShapeTypeInfo s_ShapeTypeInfo_1 = {
    FFLI_SHAPE_TYPE_OPA_HAIR_2,
    FFLI_SHAPE_TYPE_OPA_FOREHEAD_2,
    FFLI_SHAPE_TYPE_OPA_CAP_2
};

const FFLiShapeTypeInfo& GetShapeTypeInfo(FFLModelType type)
{
    switch (type)
    {
    case FFL_MODEL_TYPE_0:
        return s_ShapeTypeInfo_0;
    case FFL_MODEL_TYPE_1:
        return s_ShapeTypeInfo_1;
    }
    
    return s_ShapeTypeInfo_0;
}

static const FFLModelType s_ModelTypes[2] = {
    FFL_MODEL_TYPE_0,
    FFL_MODEL_TYPE_1
};

void SetupDrawParam(FFLiCharModel* pModel)
{
    u32 v0 = 1;

    pModel->drawParam[FFLI_SHAPE_TYPE_OPA_FACELINE]._54 = 1;
    FFLiInitModulateShapeFaceline(&pModel->drawParam[FFLI_SHAPE_TYPE_OPA_FACELINE].modulateParam, pModel->facelineRenderTexture.gx2Texture);

    pModel->drawParam[FFLI_SHAPE_TYPE_OPA_BEARD]._54 = 1;
    FFLiInitModulateShapeBeard(&pModel->drawParam[FFLI_SHAPE_TYPE_OPA_BEARD].modulateParam, pModel->charInfo.parts.beardColor);

    pModel->drawParam[FFLI_SHAPE_TYPE_OPA_NOSE]._54 = 1;
    FFLiInitModulateShapeNose(&pModel->drawParam[FFLI_SHAPE_TYPE_OPA_NOSE].modulateParam, pModel->charInfo.parts.facelineColor);

    if (pModel->charInfo.parts.hairDir > 0)
        v0 = 2;

    for (u32 i = 0; i < 2; i++)
    {
        if (pModel->charModelDesc.modelFlag & 1 << (s_ModelTypes[i] & 0x3f))
        {
            const FFLiShapeTypeInfo& shapeTypeInfo = GetShapeTypeInfo(s_ModelTypes[i]);
            
            FFLDrawParam& drawParamForehead = pModel->drawParam[shapeTypeInfo.foreheadIndex];
            drawParamForehead._54 = v0;
            FFLiInitModulateShapeForehead(&drawParamForehead.modulateParam, pModel->charInfo.parts.facelineColor);

            FFLDrawParam& drawParamHair = pModel->drawParam[shapeTypeInfo.hairIndex];
            drawParamHair._54 = v0;
            FFLiInitModulateShapeHair(&drawParamHair.modulateParam, pModel->charInfo.parts.hairColor);

            const GX2Texture* pCapTexture = pModel->pCapTexture;
            if (pCapTexture != NULL)
            {
                FFLDrawParam& drawParamCap = pModel->drawParam[shapeTypeInfo.capIndex];
                drawParamCap._54 = v0;
                FFLiInitModulateShapeCap(&drawParamCap.modulateParam, pModel->charInfo.favoriteColor, *pCapTexture);
            }
        }
    }

    const FFLiRenderTexture* pMaskRenderTexture = pModel->maskTextures.renderTextures[pModel->expression];
    if (pMaskRenderTexture != NULL)
    {
        pModel->drawParam[FFLI_SHAPE_TYPE_XLU_MASK]._54 = 1;
        FFLiInitModulateShapeMask(&pModel->drawParam[FFLI_SHAPE_TYPE_XLU_MASK].modulateParam, pMaskRenderTexture->gx2Texture);
    }

    const GX2Texture* pNoselineTexture = pModel->pNoselineTexture;
    if (pNoselineTexture != NULL)
    {
        pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE]._54 = 1;
        FFLiInitModulateShapeNoseline(&pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE].modulateParam, *pNoselineTexture);
    }

    const GX2Texture* pGlassTexture = pModel->pGlassTexture;
    if (pGlassTexture != NULL)
    {
        pModel->drawParam[FFLI_SHAPE_TYPE_XLU_GLASS]._54 = 0;
        FFLiInitModulateShapeGlass(&pModel->drawParam[FFLI_SHAPE_TYPE_XLU_GLASS].modulateParam, pModel->charInfo.parts.glassColor, *pGlassTexture);
    }
}

}

void FFLiCharModelCreator::InvalidateShapes(FFLiCharModel* pModel)
{
    for (u32 i = 0; i < FFLI_SHAPE_TYPE_MAX; i++)
        FFLiInvalidateShape(&(pModel->drawParam[i]));
}

namespace {

void InvalidateTextures(FFLiCharModel* pModel)
{
    if (pModel->pCapTexture != NULL)
        FFLiInvalidateTexture(pModel->pCapTexture);

    if (pModel->pNoselineTexture != NULL)
        FFLiInvalidateTexture(pModel->pNoselineTexture);

    if (pModel->pGlassTexture != NULL)
        FFLiInvalidateTexture(pModel->pGlassTexture);
}

}

void FFLiCharModelCreator::AfterExecuteGPUStep(FFLiCharModel* pModel)
{
    InvalidateShapes(pModel);
    InvalidateTextures(pModel);
    
    FFLiTempSetContextState(NULL);
}

void FFLiCharModelCreator::SetupRenderTextureBuffer(FFLiRenderTextureBuffer& renderTextureBuffer, const FFLCharModelDesc* pDesc, FFLiBufferAllocator* pAllocator) const
{
    std::memset(&renderTextureBuffer, 0, sizeof(FFLiRenderTextureBuffer));

    if (pDesc->enableCompressorParam)
    {
        renderTextureBuffer.size = m_pCharModelCreateParam->GetCompressBufferSize(pDesc);
        renderTextureBuffer.pBuffer = pAllocator->Allocate(renderTextureBuffer.size);
    }
}
