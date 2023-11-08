#include <nn/ffl/FFLBoundingBox.h>
#include <nn/ffl/FFLCharModelDesc.h>
#include <nn/ffl/FFLCharModelSource.h>
#include <nn/ffl/FFLModelType.h>

#include <nn/ffl/FFLiCharModel.h>
#include <nn/ffl/FFLiCharModelCreateParam.h>
#include <nn/ffl/FFLiCharModelCreator.h>
#include <nn/ffl/FFLiFacelineTexture.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiResourceLoaderBuffer.h>
#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiModulate.h>
#include <nn/ffl/FFLiShape.h>
#include <nn/ffl/FFLiShapePartsType.h>
#include <nn/ffl/FFLiShapeType.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiTextureTempObject.h>
#include <nn/ffl/FFLiUtil.h>

#include <gfx/rio_Window.h>
#include <math/rio_Matrix.h>

#if RIO_IS_CAFE
#include <gx2/event.h>
#endif // RIO_IS_CAFE

union F32BitCast
{
    f32 f;
    u32 u;
    struct
    {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        u32 sign        : 1;    // (MSB)
        u32 exponent    : 8;
        u32 mantissa    : 23;   // (LSB)
#else
        u32 mantissa    : 23;   // (LSB)
        u32 exponent    : 8;
        u32 sign        : 1;    // (MSB)
#endif // __BYTE_ORDER__
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
FFLResult InitShapes(FFLiCharModel* pModel, FFLiResourceLoader * pResLoader, const FFLiCoordinate* pCoordinate);
void DeleteShapes(FFLiCharModel* pModel);
FFLResult InitTextures(FFLiCharModel* pModel, FFLiResourceLoader* pResLoader);
void DeleteTextures(FFLiCharModel* pModel);
void AdjustPartsTransform(FFLiCharModel* pModel, const FFLiCoordinate* pCoordinate);
void SetupDrawParam(FFLiCharModel* pModel);

}

FFLResult FFLiCharModelCreator::ExecuteCPUStep(FFLiCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc)
{
    if (!FFLiCharModelCreateParam::CheckModelDesc(pDesc))
        return FFL_RESULT_ERROR;

    rio::MemUtil::set(pModel, 0, sizeof(FFLiCharModel));

    pModel->charModelDesc = *pDesc;
    pModel->modelType = ModelFlagToModelType(pModel->charModelDesc.modelFlag);

    FFLResult result = m_pCharModelCreateParam->GetDatabaseManager()->PickupCharInfo(&pModel->charInfo, pSource->dataSource, pSource->pBuffer, pSource->index);
    if (result != FFL_RESULT_OK)
        return result;

    u32 resolution = FFLiCharModelCreateParam::GetResolution(pDesc->resolution);
    bool isEnabledMipMap = FFLiCharModelCreateParam::IsEnabledMipMap(pDesc->resolution);

    FFLResourceType resourceType = pDesc->resourceType;

    if (!m_pCharModelCreateParam->GetResourceManager()->IsValid(resourceType))
        return FFL_RESULT_ERROR;

    FFLiResourceLoaderBuffer resLoaderBuffer(m_pCharModelCreateParam->GetResourceManager(), resourceType);
    FFLiResourceLoader resLoader(m_pCharModelCreateParam->GetResourceManager(), &resLoaderBuffer, resourceType);

    pModel->pTextureTempObject = new FFLiTextureTempObject;

    pModel->expression = FFLiInitMaskTextures(&pModel->maskTextures, pDesc->expressionFlag, resolution, isEnabledMipMap);

    result = FFLiInitTempObjectMaskTextures(&pModel->pTextureTempObject->maskTextures, &pModel->maskTextures, &pModel->charInfo, pDesc->expressionFlag, resolution, isEnabledMipMap, &resLoader);
    if (result != FFL_RESULT_OK)
    {
        FFLiDeleteMaskTextures(&pModel->maskTextures);
        delete pModel->pTextureTempObject;
        pModel->pTextureTempObject = NULL;
        return result;
    }

    FFLiInitFacelineTexture(&pModel->facelineRenderTexture, resolution, isEnabledMipMap);

    result = FFLiInitTempObjectFacelineTexture(&pModel->pTextureTempObject->facelineTexture, &pModel->facelineRenderTexture, &pModel->charInfo, resolution, isEnabledMipMap, &resLoader);
    if (result != FFL_RESULT_OK)
    {
        FFLiDeleteFacelineTexture(&pModel->facelineRenderTexture);
        FFLiDeleteTempObjectMaskTextures(&pModel->pTextureTempObject->maskTextures, pDesc->expressionFlag, pDesc->resourceType);
        FFLiDeleteMaskTextures(&pModel->maskTextures);
        delete pModel->pTextureTempObject;
        pModel->pTextureTempObject = NULL;
        return result;
    }

    result = InitShapes(pModel, &resLoader, &m_pCharModelCreateParam->GetCoordinate());
    if (result != FFL_RESULT_OK)
    {
        FFLiDeleteTempObjectFacelineTexture(&pModel->pTextureTempObject->facelineTexture, &pModel->charInfo, pModel->charModelDesc.resourceType);
        FFLiDeleteFacelineTexture(&pModel->facelineRenderTexture);
        FFLiDeleteTempObjectMaskTextures(&pModel->pTextureTempObject->maskTextures, pDesc->expressionFlag, pDesc->resourceType);
        FFLiDeleteMaskTextures(&pModel->maskTextures);
        delete pModel->pTextureTempObject;
        pModel->pTextureTempObject = NULL;
        return result;
    }

    result = InitTextures(pModel, &resLoader);
    if (result != FFL_RESULT_OK)
    {
        DeleteShapes(pModel);
        FFLiDeleteTempObjectFacelineTexture(&pModel->pTextureTempObject->facelineTexture, &pModel->charInfo, pModel->charModelDesc.resourceType);
        FFLiDeleteFacelineTexture(&pModel->facelineRenderTexture);
        FFLiDeleteTempObjectMaskTextures(&pModel->pTextureTempObject->maskTextures, pDesc->expressionFlag, pDesc->resourceType);
        FFLiDeleteMaskTextures(&pModel->maskTextures);
        delete pModel->pTextureTempObject;
        pModel->pTextureTempObject = NULL;
        return result;
    }

    AdjustPartsTransform(pModel, &m_pCharModelCreateParam->GetCoordinate());
    SetupDrawParam(pModel);

    return FFL_RESULT_OK;
}

void FFLiCharModelCreator::ExecuteGPUStep(FFLiCharModel* pModel, const FFLShaderCallback* pCallback)
{
    u32 resolution = FFLiCharModelCreateParam::GetResolution(pModel->charModelDesc.resolution);

    FFLiShaderCallback shaderCallback;
    shaderCallback.Set(pCallback);

    shaderCallback.CallSetMatrix(rio::Matrix44f::ident);

    FFLiRenderMaskTextures(&pModel->maskTextures, &pModel->pTextureTempObject->maskTextures, &shaderCallback
#if RIO_IS_CAFE
        , &m_pManager->GetCopySurface()
#endif // RIO_IS_CAFE
    );
    FFLiRenderFacelineTexture(&pModel->facelineRenderTexture, &pModel->charInfo, resolution, &pModel->pTextureTempObject->facelineTexture, &shaderCallback
#if RIO_IS_CAFE
        , &m_pManager->GetCopySurface()
#endif // RIO_IS_CAFE
    );

    AfterExecuteGPUStep(pModel);

    FFLiDeleteTempObjectFacelineTexture(&pModel->pTextureTempObject->facelineTexture, &pModel->charInfo, pModel->charModelDesc.resourceType);
    FFLiDeleteTempObjectMaskTextures(&pModel->pTextureTempObject->maskTextures, pModel->charModelDesc.expressionFlag, pModel->charModelDesc.resourceType);

    delete pModel->pTextureTempObject;
    pModel->pTextureTempObject = NULL;
}

void FFLiCharModelCreator::Delete(FFLiCharModel* pModel)
{
#if RIO_IS_CAFE
    GX2DrawDone();
#elif RIO_IS_WIN
    RIO_GL_CALL(glFinish());
#endif

    DeleteTextures(pModel);
    DeleteShapes(pModel);
    FFLiDeleteFacelineTexture(&pModel->facelineRenderTexture);

    if (pModel->pTextureTempObject != NULL)
    {
        FFLiDeleteTempObjectFacelineTexture(&pModel->pTextureTempObject->facelineTexture, &pModel->charInfo, pModel->charModelDesc.resourceType);
        FFLiDeleteTempObjectMaskTextures(&pModel->pTextureTempObject->maskTextures, pModel->charModelDesc.expressionFlag, pModel->charModelDesc.resourceType);

        delete pModel->pTextureTempObject;
        pModel->pTextureTempObject = NULL;
    }

    FFLiDeleteMaskTextures(&pModel->maskTextures);
}

namespace {

FFLModelType ModelFlagToModelType(u32 flag)
{
    for (u32 i = 0; i < FFL_MODEL_TYPE_MAX; ++i)
        if (flag & 1 << i)
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
    default:
        return FFLI_SHAPE_TYPE_MAX;
    }
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

static const bool UPDATE_BOUNDING_BOX[FFLI_SHAPE_PARTS_TYPE_MAX][3] = {
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
        if (UPDATE_BOUNDING_BOX[partsType][i])
            UpdateBoundingBox(&(pDst[i]), pSrc);
}

FFLResult InitShape(FFLiCharModel* pModel, FFLiShapePartsType partsType, u32 index, f32 scaleX, f32 scaleY, const FFLVec3* pTranslate, bool flipX, FFLiResourceLoader* pResLoader, const FFLiCoordinate* pCoordinate)
{
    FFLiShapeType type = ConvertShapePartsTypeToShapeType(partsType);
    FFLDrawParam* pDrawParam = &(pModel->drawParam[type]);
    FFLBoundingBox boundingBox;

    FFLResult result = FFLiLoadShape(pDrawParam, &boundingBox, pModel, partsType, index, pResLoader);
    if (result != FFL_RESULT_OK)
        return result;

    FFLiAdjustShape(pDrawParam, &boundingBox, scaleX, scaleY, pTranslate, flipX, pCoordinate, partsType, pModel->charModelDesc.modelFlag >> 3 & 1);
    CalcluateBoundingBox(pModel->boundingBox, &boundingBox, partsType);
    return FFL_RESULT_OK;
}

void DeleteShape(FFLiCharModel* pModel, FFLiShapePartsType partsType)
{
    FFLiShapeType type = ConvertShapePartsTypeToShapeType(partsType);
    FFLDrawParam* pDrawParam = &(pModel->drawParam[type]);

    FFLiDeleteShape(pDrawParam);
}

void DeleteShape_Faceline(FFLiCharModel* pModel)
{
    DeleteShape(pModel, FFLI_SHAPE_PARTS_TYPE_FACELINE);
}

struct ModelTypeShapePartsInfo
{
    bool                enable;
    FFLiShapePartsType  partsType;
};

const ModelTypeShapePartsInfo* GetModelTypeShapePartsInfo(u32 modelFlag)
{
    static ModelTypeShapePartsInfo modelTypeShapePartsInfo[2 * 3] = {
        // FFL_MODEL_TYPE_0
        { false, FFLI_SHAPE_PARTS_TYPE_HAIR_1 },
        { false, FFLI_SHAPE_PARTS_TYPE_CAP_1 },
        { false, FFLI_SHAPE_PARTS_TYPE_FOREHEAD_1 },
        // FFL_MODEL_TYPE_1
        { false, FFLI_SHAPE_PARTS_TYPE_HAIR_2 },
        { false, FFLI_SHAPE_PARTS_TYPE_CAP_2 },
        { false, FFLI_SHAPE_PARTS_TYPE_FOREHEAD_2 }
    };

    bool modelType0Enable = modelFlag & 1 << FFL_MODEL_TYPE_0;
    bool modelType1Enable = modelFlag & 1 << FFL_MODEL_TYPE_1;

    modelTypeShapePartsInfo[0 * 3 + 0].enable = modelType0Enable;
    modelTypeShapePartsInfo[0 * 3 + 1].enable = modelType0Enable;
    modelTypeShapePartsInfo[0 * 3 + 2].enable = modelType0Enable;

    modelTypeShapePartsInfo[1 * 3 + 0].enable = modelType1Enable;
    modelTypeShapePartsInfo[1 * 3 + 1].enable = modelType1Enable;
    modelTypeShapePartsInfo[1 * 3 + 2].enable = modelType1Enable;

    return modelTypeShapePartsInfo;
}

void DeleteShape_Hair(FFLiCharModel* pModel, u32 count = 2 * 3)
{
    u32 modelFlag = pModel->charModelDesc.modelFlag & 7;

    if (modelFlag & (1 << FFL_MODEL_TYPE_0 |
                     1 << FFL_MODEL_TYPE_1))
    {
        const ModelTypeShapePartsInfo* modelTypeShapePartsInfo = GetModelTypeShapePartsInfo(modelFlag);
        for (u32 j = count; j > 0; j--)
            DeleteShape(pModel, modelTypeShapePartsInfo[j - 1].partsType);
    }
}

void DeleteShape_Beard(FFLiCharModel* pModel)
{
    if (pModel->charInfo.parts.beardType < 4)
        DeleteShape(pModel, FFLI_SHAPE_PARTS_TYPE_BEARD);
}

void DeleteShape_Nose(FFLiCharModel* pModel)
{
    DeleteShape(pModel, FFLI_SHAPE_PARTS_TYPE_NOSE);
}

void DeleteShape_Noseline(FFLiCharModel* pModel)
{
    DeleteShape(pModel, FFLI_SHAPE_PARTS_TYPE_NOSELINE);
}

void DeleteShape_Mask(FFLiCharModel* pModel)
{
    DeleteShape(pModel, FFLI_SHAPE_PARTS_TYPE_MASK);
}

void DeleteShape_Glass(FFLiCharModel* pModel)
{
    if (pModel->charInfo.parts.glassType > 0)
        DeleteShape(pModel, FFLI_SHAPE_PARTS_TYPE_GLASS);
}

FFLResult InitShapes(FFLiCharModel* pModel, FFLiResourceLoader * pResLoader, const FFLiCoordinate* pCoordinate)
{
    u32 modelFlag = pModel->charModelDesc.modelFlag & 7;

    FFLResult result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_FACELINE, pModel->charInfo.parts.faceType, 1.0f, 1.0f, NULL, false, pResLoader, pCoordinate);
    if (result != FFL_RESULT_OK)
        return result;

    if (modelFlag & (1 << FFL_MODEL_TYPE_0 |
                     1 << FFL_MODEL_TYPE_1))
    {
        const ModelTypeShapePartsInfo* modelTypeShapePartsInfo = GetModelTypeShapePartsInfo(modelFlag);

        bool flipHair = pModel->charInfo.parts.hairDir > 0;

        for (u32 i = 0; i < 2 * 3; i++)
        {
            if (modelTypeShapePartsInfo[i].enable)
            {
                result = InitShape(pModel, modelTypeShapePartsInfo[i].partsType, pModel->charInfo.parts.hairType, 1.0f, 1.0f, &pModel->hairPos, flipHair, pResLoader, pCoordinate);
                if (result != FFL_RESULT_OK)
                {
                    DeleteShape_Hair(pModel, i);
                    DeleteShape_Faceline(pModel);
                    return result;
                }
            }
        }

    }

    if (pModel->charInfo.parts.beardType < 4)
    {
        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_BEARD, pModel->charInfo.parts.beardType, 1.0f, 1.0f, &pModel->beardPos, false, pResLoader, pCoordinate);
        if (result != FFL_RESULT_OK)
        {
            DeleteShape_Hair(pModel);
            DeleteShape_Faceline(pModel);
            return result;
        }
    }

    {
        f32 noseScale = pModel->charInfo.parts.noseScale * 0.175f + 0.4f;

        FFLVec3 nosePos;
        nosePos.x = pModel->faceCenterPos.x;
        nosePos.y = pModel->faceCenterPos.y + (pModel->charInfo.parts.nosePositionY - 8) * -1.5f;
        nosePos.z = pModel->faceCenterPos.z;

        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_NOSE, pModel->charInfo.parts.noseType, noseScale, noseScale, &nosePos, false, pResLoader, pCoordinate);
        if (result != FFL_RESULT_OK)
        {
            DeleteShape_Beard(pModel);
            DeleteShape_Hair(pModel);
            DeleteShape_Faceline(pModel);
            return result;
        }

        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_NOSELINE, pModel->charInfo.parts.noseType, noseScale, noseScale, &nosePos, false, pResLoader, pCoordinate);
        if (result != FFL_RESULT_OK)
        {
            DeleteShape_Nose(pModel);
            DeleteShape_Beard(pModel);
            DeleteShape_Hair(pModel);
            DeleteShape_Faceline(pModel);
            return result;
        }
    }

    result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_MASK, pModel->charInfo.parts.faceType, 1.0f, 1.0f, NULL, false, pResLoader, pCoordinate);
    if (result != FFL_RESULT_OK)
    {
        DeleteShape_Noseline(pModel);
        DeleteShape_Nose(pModel);
        DeleteShape_Beard(pModel);
        DeleteShape_Hair(pModel);
        DeleteShape_Faceline(pModel);
        return result;
    }

    if (pModel->charInfo.parts.glassType > 0)
    {
        f32 glassScale = pModel->charInfo.parts.glassScale * 0.15f + 0.4f;

        FFLVec3 glassPos;
        glassPos.x = pModel->faceCenterPos.x;
        glassPos.y = pModel->faceCenterPos.y + (pModel->charInfo.parts.glassPositionY - 11) * -1.5f + 5.0f;
        glassPos.z = pModel->faceCenterPos.z + 2.0f;

        result = InitShape(pModel, FFLI_SHAPE_PARTS_TYPE_GLASS, 0, glassScale, glassScale, &glassPos, false, pResLoader, pCoordinate);
        if (result != FFL_RESULT_OK)
        {
            DeleteShape_Mask(pModel);
            DeleteShape_Noseline(pModel);
            DeleteShape_Nose(pModel);
            DeleteShape_Beard(pModel);
            DeleteShape_Hair(pModel);
            DeleteShape_Faceline(pModel);
            return result;
        }
    }

    return FFL_RESULT_OK;
}

void DeleteShapes(FFLiCharModel* pModel)
{
    DeleteShape_Glass(pModel);
    DeleteShape_Mask(pModel);
    DeleteShape_Noseline(pModel);
    DeleteShape_Nose(pModel);
    DeleteShape_Beard(pModel);
    DeleteShape_Hair(pModel);
    DeleteShape_Faceline(pModel);
}

void DeleteTexture_Cap(FFLiCharModel* pModel, bool isExpand)
{
    if (FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_CAP_1])) ||
        FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_CAP_2])))
    {
        FFLiDeleteTexture(&pModel->pCapTexture, isExpand);
    }
}

void DeleteTexture_Noseline(FFLiCharModel* pModel, bool isExpand)
{
    if (FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE])))
        FFLiDeleteTexture(&pModel->pNoselineTexture, isExpand);
}

void DeleteTexture_Glass(FFLiCharModel* pModel, bool isExpand)
{
    if (pModel->charInfo.parts.glassType > 0)
        FFLiDeleteTexture(&pModel->pGlassTexture, isExpand);
}

FFLResult InitTextures(FFLiCharModel* pModel, FFLiResourceLoader* pResLoader)
{
    if (FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_CAP_1])) ||
        FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_CAP_2])))
    {
        FFLResult result = FFLiLoadTextureWithAllocate(&pModel->pCapTexture, FFLI_TEXTURE_PARTS_TYPE_CAP, pModel->charInfo.parts.hairType, pResLoader);
        if (result != FFL_RESULT_OK)
            return result;
    }

    if (FFLiCanDrawShape(&(pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE])))
    {
        FFLResult result = FFLiLoadTextureWithAllocate(&pModel->pNoselineTexture, FFLI_TEXTURE_PARTS_TYPE_NOSELINE, pModel->charInfo.parts.noseType, pResLoader);
        if (result != FFL_RESULT_OK)
        {
            DeleteTexture_Cap(pModel, pResLoader->IsExpand());
            return result;
        }
    }

    if (pModel->charInfo.parts.glassType > 0)
    {
        FFLResult result = FFLiLoadTextureWithAllocate(&pModel->pGlassTexture, FFLI_TEXTURE_PARTS_TYPE_GLASS, pModel->charInfo.parts.glassType, pResLoader);
        if (result != FFL_RESULT_OK)
        {
            DeleteTexture_Noseline(pModel, pResLoader->IsExpand());
            DeleteTexture_Cap(pModel, pResLoader->IsExpand());
            return result;
        }
    }

    return FFL_RESULT_OK;
}

void DeleteTextures(FFLiCharModel* pModel)
{
    RIO_ASSERT(FFLiManager::IsConstruct());
    bool isExpand = FFLiManager::GetInstance()->GetResourceManager().IsExpand(pModel->charModelDesc.resourceType);

    DeleteTexture_Glass(pModel, isExpand);
    DeleteTexture_Noseline(pModel, isExpand);
    DeleteTexture_Cap(pModel, isExpand);
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

static const FFLiShapeTypeInfo SHAPE_TYPE_INFO_0 = {
    FFLI_SHAPE_TYPE_OPA_HAIR_1,
    FFLI_SHAPE_TYPE_OPA_FOREHEAD_1,
    FFLI_SHAPE_TYPE_OPA_CAP_1
};

static const FFLiShapeTypeInfo SHAPE_TYPE_INFO_1 = {
    FFLI_SHAPE_TYPE_OPA_HAIR_2,
    FFLI_SHAPE_TYPE_OPA_FOREHEAD_2,
    FFLI_SHAPE_TYPE_OPA_CAP_2
};

const FFLiShapeTypeInfo& GetShapeTypeInfo(FFLModelType type)
{
    switch (type)
    {
    case FFL_MODEL_TYPE_0:
        return SHAPE_TYPE_INFO_0;
    case FFL_MODEL_TYPE_1:
        return SHAPE_TYPE_INFO_1;
    default:
        return SHAPE_TYPE_INFO_0;
    }
}

static const FFLModelType MODEL_TYPE[2] = {
    FFL_MODEL_TYPE_0,
    FFL_MODEL_TYPE_1
};

void SetupDrawParam(FFLiCharModel* pModel)
{
    FFLCullMode hairCullMode = FFL_CULL_MODE_BACK;

    pModel->drawParam[FFLI_SHAPE_TYPE_OPA_FACELINE].cullMode = FFL_CULL_MODE_BACK;
    FFLiInitModulateShapeFaceline(&pModel->drawParam[FFLI_SHAPE_TYPE_OPA_FACELINE].modulateParam, *pModel->facelineRenderTexture.pTextureData);

    pModel->drawParam[FFLI_SHAPE_TYPE_OPA_BEARD].cullMode = FFL_CULL_MODE_BACK;
    FFLiInitModulateShapeBeard(&pModel->drawParam[FFLI_SHAPE_TYPE_OPA_BEARD].modulateParam, pModel->charInfo.parts.beardColor);

    pModel->drawParam[FFLI_SHAPE_TYPE_OPA_NOSE].cullMode = FFL_CULL_MODE_BACK;
    FFLiInitModulateShapeNose(&pModel->drawParam[FFLI_SHAPE_TYPE_OPA_NOSE].modulateParam, pModel->charInfo.parts.facelineColor);

    if (pModel->charInfo.parts.hairDir > 0)
        hairCullMode = FFL_CULL_MODE_FRONT;

    for (u32 i = 0; i < 2; i++)
    {
        if (pModel->charModelDesc.modelFlag & 1 << MODEL_TYPE[i])
        {
            const FFLiShapeTypeInfo& shapeTypeInfo = GetShapeTypeInfo(MODEL_TYPE[i]);

            FFLDrawParam& drawParamForehead = pModel->drawParam[shapeTypeInfo.foreheadIndex];
            drawParamForehead.cullMode = hairCullMode;
            FFLiInitModulateShapeForehead(&drawParamForehead.modulateParam, pModel->charInfo.parts.facelineColor);

            FFLDrawParam& drawParamHair = pModel->drawParam[shapeTypeInfo.hairIndex];
            drawParamHair.cullMode = hairCullMode;
            FFLiInitModulateShapeHair(&drawParamHair.modulateParam, pModel->charInfo.parts.hairColor);

            const agl::TextureData* pCapTexture = pModel->pCapTexture;
            if (pCapTexture != NULL)
            {
                FFLDrawParam& drawParamCap = pModel->drawParam[shapeTypeInfo.capIndex];
                drawParamCap.cullMode = hairCullMode;
                FFLiInitModulateShapeCap(&drawParamCap.modulateParam, pModel->charInfo.favoriteColor, *pCapTexture);
            }
        }
    }

    const FFLiRenderTexture* pMaskRenderTexture = pModel->maskTextures.pRenderTextures[pModel->expression];
    if (pMaskRenderTexture != NULL)
    {
        pModel->drawParam[FFLI_SHAPE_TYPE_XLU_MASK].cullMode = FFL_CULL_MODE_BACK;
        FFLiInitModulateShapeMask(&pModel->drawParam[FFLI_SHAPE_TYPE_XLU_MASK].modulateParam, *pMaskRenderTexture->pTextureData);
    }

    const agl::TextureData* pNoselineTexture = pModel->pNoselineTexture;
    if (pNoselineTexture != NULL)
    {
        pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE].cullMode = FFL_CULL_MODE_BACK;
        FFLiInitModulateShapeNoseline(&pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE].modulateParam, *pNoselineTexture);
    }

    const agl::TextureData* pGlassTexture = pModel->pGlassTexture;
    if (pGlassTexture != NULL)
    {
        pModel->drawParam[FFLI_SHAPE_TYPE_XLU_GLASS].cullMode = FFL_CULL_MODE_NONE;
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
        pModel->pCapTexture->invalidateGPUCache();

    if (pModel->pNoselineTexture != NULL)
        pModel->pNoselineTexture->invalidateGPUCache();

    if (pModel->pGlassTexture != NULL)
        pModel->pGlassTexture->invalidateGPUCache();
}

}

void FFLiCharModelCreator::AfterExecuteGPUStep(FFLiCharModel* pModel)
{
    InvalidateShapes(pModel);
    InvalidateTextures(pModel);

    rio::Window::instance()->makeContextCurrent();

    u32 width = rio::Window::instance()->getWidth();
    u32 height = rio::Window::instance()->getHeight();

    rio::Graphics::setViewport(0, 0, width, height);
    rio::Graphics::setScissor(0, 0, width, height);

#if RIO_IS_CAFE
    GX2DrawDone();
#elif RIO_IS_WIN
    RIO_GL_CALL(glFinish());
#endif
}
