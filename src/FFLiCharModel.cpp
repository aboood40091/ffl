#include <nn/ffl/FFLiCharModel.h>
#include <nn/ffl/FFLiCharModelCreator.h>
#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiModulate.h>

FFLResult FFLiInitCharModelCPUStep(FFLiCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc)
{
    if (!FFLiManager::IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    FFLiManager* pManager = FFLiManager::GetInstance();
    if (!pManager->CanInitCharModel())
        return FFL_RESULT_ERROR;

    FFLiCharModelCreator creator(&pManager->GetCharModelCreateParam(), pManager);
    return creator.ExecuteCPUStep(pModel, pSource, pDesc);
}

void FFLiInitCharModelGPUStep(FFLiCharModel* pModel, const FFLShaderCallback* pCallback)
{
    if (!FFLiManager::IsConstruct())
        return;

    FFLiManager* pManager = FFLiManager::GetInstance();
    FFLiCharModelCreator creator(&pManager->GetCharModelCreateParam(), pManager);
    return creator.ExecuteGPUStep(pModel, pCallback);
}

void FFLiDeleteCharModel(FFLiCharModel* pModel)
{
    (void)pModel;
}

void FFLiSetExpression(FFLiCharModel* pModel, FFLExpression expression)
{
    if (1 << expression & pModel->charModelDesc.expressionFlag)
    {
        pModel->expression = expression;
        FFLiInitModulateShapeMask(&pModel->drawParam[FFLI_SHAPE_TYPE_XLU_MASK].modulateParam, pModel->maskTextures.pRenderTextures[expression]->textureData);
    }
}

FFLExpression FFLiGetExpression(const FFLiCharModel* pModel)
{
    return pModel->expression;
}

void FFLiGetPartsTransform(FFLPartsTransform* pTransform, const FFLiCharModel* pModel)
{
    *pTransform = pModel->partsTransform;
}

void FFLiSetViewModelType(FFLiCharModel* pModel, FFLModelType type)
{
    if (type < FFL_MODEL_TYPE_MAX &&
        (pModel->charModelDesc).modelFlag & 1 << type)
    {
        pModel->modelType = type;
    }
}

const FFLDrawParam* FFLiGetDrawParamOpaFacelineFromCharModel(const FFLiCharModel* pModel)
{
    return &(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_FACELINE]);
}

const FFLDrawParam* FFLiGetDrawParamOpaBeardFromCharModel(const FFLiCharModel* pModel)
{
    return &(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_BEARD]);
}

const FFLDrawParam* FFLiGetDrawParamOpaNoseFromCharModel(const FFLiCharModel* pModel)
{
    return &(pModel->drawParam[FFLI_SHAPE_TYPE_OPA_NOSE]);
}

namespace {

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

}

const FFLDrawParam* FFLiGetDrawParamOpaForeheadFromCharModel(const FFLiCharModel* pModel)
{
    const FFLDrawParam* pDrawParam = NULL;
    if (pModel->modelType != FFL_MODEL_TYPE_2)
    {
        const FFLDrawParam* drawParam = pModel->drawParam;
        pDrawParam = &(drawParam[GetShapeTypeInfo(pModel->modelType).foreheadIndex]);
    }
    return pDrawParam;
}

const FFLDrawParam* FFLiGetDrawParamOpaHairFromCharModel(const FFLiCharModel* pModel)
{
    const FFLDrawParam* pDrawParam = NULL;
    if (pModel->modelType != FFL_MODEL_TYPE_2)
    {
        const FFLDrawParam* drawParam = pModel->drawParam;
        pDrawParam = &(drawParam[GetShapeTypeInfo(pModel->modelType).hairIndex]);
    }
    return pDrawParam;
}

const FFLDrawParam* FFLiGetDrawParamOpaCapFromCharModel(const FFLiCharModel* pModel)
{
    if (pModel->modelType == FFL_MODEL_TYPE_2 || pModel->pCapTexture == NULL)
        return NULL;

    const FFLDrawParam* drawParam = pModel->drawParam;
    return &(drawParam[GetShapeTypeInfo(pModel->modelType).capIndex]);
}

const FFLDrawParam* FFLiGetDrawParamXluMaskFromCharModel(const FFLiCharModel* pModel)
{
    return &(pModel->drawParam[FFLI_SHAPE_TYPE_XLU_MASK]);
}

const FFLDrawParam* FFLiGetDrawParamXluNoseLineFromCharModel(const FFLiCharModel* pModel)
{
    return
        pModel->pNoselineTexture == NULL
            ? NULL
            : &(pModel->drawParam[FFLI_SHAPE_TYPE_XLU_NOSELINE]);
}

const FFLDrawParam* FFLiGetDrawParamXluGlassFromCharModel(const FFLiCharModel* pModel)
{
    return
        pModel->pGlassTexture == NULL
            ? NULL
            : &(pModel->drawParam[FFLI_SHAPE_TYPE_XLU_GLASS]);
}
