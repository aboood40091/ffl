#ifndef FFLI_CHAR_MODEL_H_
#define FFLI_CHAR_MODEL_H_

#include <nn/ffl/FFLCharModelDesc.h>
#include <nn/ffl/FFLDrawParam.h>
#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLModelType.h>
#include <nn/ffl/FFLPartsTransform.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiMaskTextures.h>
#include <nn/ffl/FFLiRenderTexture.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

struct FFLiCharModel
{
    FFLiCharInfo        charInfo;
    FFLCharModelDesc    charModelDesc;
    u32                 _134;
    u32                 _138;
    FFLExpression       expression;
    void*               pMaskTexturesTempObject;
    union
    {
        struct
        {
            FFLDrawParam    drawParamOpaBeard;
            FFLDrawParam    drawParamOpaFaceline;
            FFLDrawParam    drawParamOpaHair1;
            FFLDrawParam    drawParamOpaForehead1;
            FFLDrawParam    drawParamXluMask;
            FFLDrawParam    drawParamXluNoseLine;
            FFLDrawParam    drawParamOpaNose;
            FFLDrawParam    drawParamOpaCap1;
            FFLDrawParam    drawParamXluGlass;
            FFLDrawParam    drawParamOpaHair2;
            FFLDrawParam    drawParamOpaForehead2;
            FFLDrawParam    drawParamOpaCap2;
        };
        FFLDrawParam    drawParam[12];
    };
    FFLiRenderTexture   facelineRenderTexture;
    FFLiRenderTexture*  pCapRenderTexture;
    FFLiRenderTexture*  pGlassRenderTexture;
    FFLiRenderTexture*  pNoselineRenderTexture;
    FFLiMaskTextures    maskTextures;
    u32                 _718[36 / sizeof(u32)];
    FFLPartsTransform   partsTransform;
    FFLModelType        modelType;
    u32                 _794[72 / sizeof(u32)];
};
NN_STATIC_ASSERT(sizeof(FFLiCharModel) == 0x7DC);

struct FFLCharModelSource;
struct FFLCharModelBuffer;
struct FFLShaderCallback;

u32 FFLiGetBufferSizeCharModel(const FFLCharModelDesc* pDesc);

FFLResult FFLiInitCharModelCPUStep(FFLiCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc, FFLCharModelBuffer* pBuffer);

u32 FFLiGetTempBufferSizeCharModel(const FFLCharModelDesc* pDesc);

void FFLiInitCharModelGPUStep(FFLiCharModel* pModel, const FFLShaderCallback* pCallback);

void FFLiDeleteCharModel(FFLiCharModel* pModel);

void FFLiSetExpression(FFLiCharModel* pModel, FFLExpression expression);
FFLExpression FFLiGetExpression(const FFLiCharModel* pModel);

void FFLiGetPartsTransform(FFLPartsTransform* pTransform, const FFLiCharModel* pModel);

void FFLiSetViewModelType(FFLiCharModel* pModel, FFLModelType type);

const FFLDrawParam* FFLiGetDrawParamOpaFacelineFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaBeardFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaNoseFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaForeheadFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaHairFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaCapFromCharModel(const FFLiCharModel* pModel);

const FFLDrawParam* FFLiGetDrawParamXluMaskFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamXluNoseLineFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamXluGlassFromCharModel(const FFLiCharModel* pModel);

#endif // FFLI_CHAR_MODEL_H_
