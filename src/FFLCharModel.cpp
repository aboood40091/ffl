#include <nn/ffl/FFLCharModel.h>

#include <nn/ffl/FFLiCharModel.h>
#include <nn/ffl/FFLiManager.h>

u32 FFLGetBufferSizeCharModel(const FFLCharModelDesc* pDesc)
{
    return FFLiGetBufferSizeCharModel(pDesc);
}

u32 FFLGetTempBufferSizeCharModel(const FFLCharModelDesc* pDesc)
{
    return FFLiGetTempBufferSizeCharModel(pDesc);
}

namespace {

FFLiCharModel* GetCharModel(FFLCharModel* pModel)
{
    return reinterpret_cast<FFLiCharModel*>(pModel);
}

}

FFLResult FFLInitCharModelCPUStep(FFLCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc, FFLCharModelBuffer* pBuffer)
{
    return FFLiInitCharModelCPUStep(GetCharModel(pModel), pSource, pDesc, pBuffer);
}

void FFLInitCharModelGPUStep(FFLCharModel* pModel)
{
    const FFLShaderCallback* pCallback = NULL;
    if (FFLiManager::IsConstruct())
        pCallback = FFLiManager::GetInstance()->GetShaderCallback().Get();

    FFLInitCharModelGPUStepWithCallback(pModel, pCallback);
}

void FFLInitCharModelGPUStepWithCallback(FFLCharModel* pModel, const FFLShaderCallback* pCallback)
{
    FFLiInitCharModelGPUStep(GetCharModel(pModel), pCallback);
}

void FFLDeleteCharModel(FFLCharModel* pModel)
{
    FFLiDeleteCharModel(GetCharModel(pModel));
}

namespace {

const FFLiCharModel* GetCharModel(const FFLCharModel* pModel)
{
    return reinterpret_cast<const FFLiCharModel*>(pModel);
}

}

const FFLDrawParam* FFLGetDrawParamOpaFaceline(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamOpaFacelineFromCharModel(GetCharModel(pModel));
}

const FFLDrawParam* FFLGetDrawParamOpaBeard(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamOpaBeardFromCharModel(GetCharModel(pModel));
}

const FFLDrawParam* FFLGetDrawParamOpaNose(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamOpaNoseFromCharModel(GetCharModel(pModel));
}

const FFLDrawParam* FFLGetDrawParamOpaForehead(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamOpaForeheadFromCharModel(GetCharModel(pModel));
}

const FFLDrawParam* FFLGetDrawParamOpaHair(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamOpaHairFromCharModel(GetCharModel(pModel));
}

const FFLDrawParam* FFLGetDrawParamOpaCap(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamOpaCapFromCharModel(GetCharModel(pModel));
}

void FFLDrawOpa(const FFLCharModel* pModel)
{
    const FFLShaderCallback* pCallback = NULL;
    if (FFLiManager::IsConstruct())
        pCallback = FFLiManager::GetInstance()->GetShaderCallback().Get();

    FFLDrawOpaWithCallback(pModel, pCallback);
}

void FFLDrawOpaWithCallback(const FFLCharModel* pModel, const FFLShaderCallback* pCallback)
{
    FFLiShaderCallback shaderCallback;
    shaderCallback.Set(pCallback);

    const FFLDrawParam* pDrawParam;

    pDrawParam = FFLGetDrawParamOpaFaceline(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
        
    pDrawParam = FFLGetDrawParamOpaBeard(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
        
    pDrawParam = FFLGetDrawParamOpaNose(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
        
    pDrawParam = FFLGetDrawParamOpaForehead(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
        
    pDrawParam = FFLGetDrawParamOpaHair(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
    
    pDrawParam = FFLGetDrawParamOpaCap(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
}

const FFLDrawParam* FFLGetDrawParamXluMask(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamXluMaskFromCharModel(GetCharModel(pModel));
}

const FFLDrawParam* FFLGetDrawParamXluNoseLine(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamXluNoseLineFromCharModel(GetCharModel(pModel));
}

const FFLDrawParam* FFLGetDrawParamXluGlass(const FFLCharModel* pModel)
{
    return FFLiGetDrawParamXluGlassFromCharModel(GetCharModel(pModel));
}

void FFLDrawXlu(const FFLCharModel* pModel)
{
    const FFLShaderCallback* pCallback = NULL;
    if (FFLiManager::IsConstruct())
        pCallback = FFLiManager::GetInstance()->GetShaderCallback().Get();

    FFLDrawXluWithCallback(pModel, pCallback);
}

void FFLDrawXluWithCallback(const FFLCharModel* pModel, const FFLShaderCallback* pCallback)
{
    FFLiShaderCallback shaderCallback;
    shaderCallback.Set(pCallback);

    const FFLDrawParam* pDrawParam;

    pDrawParam = FFLGetDrawParamXluMask(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
        
    pDrawParam = FFLGetDrawParamXluNoseLine(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
        
    pDrawParam = FFLGetDrawParamXluGlass(pModel);
    if (pDrawParam != NULL)
        shaderCallback.CallDraw(*pDrawParam);
}

void FFLSetExpression(FFLCharModel* pModel, FFLExpression expression)
{
    FFLiSetExpression(GetCharModel(pModel), expression);
}

FFLExpression FFLGetExpression(const FFLCharModel* pModel)
{
    return FFLiGetExpression(GetCharModel(pModel));
}

void FFLGetPartsTransform(FFLPartsTransform* pTransform, const FFLCharModel* pModel)
{
    FFLiGetPartsTransform(pTransform, GetCharModel(pModel));
}

void FFLSetViewModelType(FFLCharModel* pModel, FFLModelType type)
{
    FFLiSetViewModelType(GetCharModel(pModel), type);
}
