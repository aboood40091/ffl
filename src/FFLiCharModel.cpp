#include <nn/ffl/FFLiCharModel.h>
#include <nn/ffl/FFLiCharModelCreator.h>
#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiModulate.h>

u32 FFLiGetBufferSizeCharModel(const FFLCharModelDesc* pDesc)
{
    if (FFLiManager::IsConstruct())
        return FFLiManager::GetInstance()->GetCharModelCreateParam().GetBufferSize(pDesc);

    return 0;
}

FFLResult FFLiInitCharModelCPUStep(FFLiCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc, FFLCharModelBuffer* pBuffer)
{
    if (!FFLiManager::IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    FFLiManager* pManager = FFLiManager::GetInstance();
    if (!pManager->CanInitCharModel())
        return FFL_RESULT_ERROR;

    FFLiCharModelCreator creator(&pManager->GetCharModelCreateParam(), pManager);
    return creator.ExecuteCPUStep(pModel, pSource, pDesc, pBuffer);
}

u32 FFLiGetTempBufferSizeCharModel(const FFLCharModelDesc* pDesc)
{
    if (FFLiManager::IsConstruct())
        return FFLiManager::GetInstance()->GetCharModelCreateParam().GetTempBufferSize(pDesc);

    return 0;
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
    if (1 << (expression & 0x3f) & pModel->charModelDesc.expressionFlag)
    {
        pModel->expression = expression;
        FFLiInitModulateShapeMask(&pModel->drawParamXluMask.modulateParam, pModel->maskTextures.renderTextures[expression]->gx2Texture);
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
        (pModel->charModelDesc).modelFlag & 1 << (type & 0x3f))
    {
        pModel->modelType = type;
    }
}

const FFLDrawParam* FFLiGetDrawParamOpaFacelineFromCharModel(const FFLiCharModel* pModel)
{
    return &pModel->drawParamOpaFaceline;
}

const FFLDrawParam* FFLiGetDrawParamOpaBeardFromCharModel(const FFLiCharModel* pModel)
{
    return &pModel->drawParamOpaBeard;
}

const FFLDrawParam* FFLiGetDrawParamOpaNoseFromCharModel(const FFLiCharModel* pModel)
{
    return &pModel->drawParamOpaNose;
}

struct FFLiShapeTypeInfo
{
    u32 hairIndex;
    u32 foreheadIndex;
    u32 capIndex;
};

static const FFLiShapeTypeInfo s_ShapeTypeInfo_0 = {
    2,
    3,
    7
};

static const FFLiShapeTypeInfo s_ShapeTypeInfo_1 = {
    9,
    10,
    11
};

namespace {

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

}

const FFLDrawParam* FFLiGetDrawParamOpaForeheadFromCharModel(const FFLiCharModel* pModel)
{
    const FFLDrawParam* pParam = NULL;
    if (pModel->modelType != FFL_MODEL_TYPE_2)
    {
        const FFLDrawParam* drawParam = pModel->drawParam;
        pParam = &(drawParam[GetShapeTypeInfo(pModel->modelType).foreheadIndex]);
    }
    return pParam;
}

const FFLDrawParam* FFLiGetDrawParamOpaHairFromCharModel(const FFLiCharModel* pModel)
{
    const FFLDrawParam* pParam = NULL;
    if (pModel->modelType != FFL_MODEL_TYPE_2)
    {
        const FFLDrawParam* drawParam = pModel->drawParam;
        pParam = &(drawParam[GetShapeTypeInfo(pModel->modelType).hairIndex]);
    }
    return pParam;
}

const FFLDrawParam* FFLiGetDrawParamOpaCapFromCharModel(const FFLiCharModel* pModel)
{
    if (pModel->modelType == FFL_MODEL_TYPE_2 || pModel->pCapRenderTexture == NULL)
        return NULL;

    const FFLDrawParam* drawParam = pModel->drawParam;
    return &(drawParam[GetShapeTypeInfo(pModel->modelType).capIndex]);
}

const FFLDrawParam* FFLiGetDrawParamXluMaskFromCharModel(const FFLiCharModel* pModel)
{
    return &pModel->drawParamXluMask;
}

const FFLDrawParam* FFLiGetDrawParamXluNoseLineFromCharModel(const FFLiCharModel* pModel)
{
    return
        pModel->pNoselineRenderTexture == NULL
            ? NULL
            : &pModel->drawParamXluNoseLine;
}

const FFLDrawParam* FFLiGetDrawParamXluGlassFromCharModel(const FFLiCharModel* pModel)
{
    return
        pModel->pGlassRenderTexture == NULL
            ? NULL
            : &pModel->drawParamXluGlass;
}
