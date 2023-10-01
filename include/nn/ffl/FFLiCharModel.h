#ifndef FFLI_CHAR_MODEL_H_
#define FFLI_CHAR_MODEL_H_

#include <nn/ffl/FFLBoundingBox.h>
#include <nn/ffl/FFLCharModelDesc.h>
#include <nn/ffl/FFLDrawParam.h>
#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLModelType.h>
#include <nn/ffl/FFLPartsTransform.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiMaskTextures.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiShapeType.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

struct FFLiTextureTempObject;

struct FFLiCharModel
{
    FFLiCharInfo            charInfo;
    FFLCharModelDesc        charModelDesc;
    void*                   pBuffer;
    u32                     bufferSize;
    FFLExpression           expression;
    FFLiTextureTempObject*  pTextureTempObject;
    FFLDrawParam            drawParam[FFLI_SHAPE_TYPE_MAX];
    FFLiRenderTexture       facelineRenderTexture;
    GX2Texture*             pCapTexture;
    GX2Texture*             pGlassTexture;
    GX2Texture*             pNoselineTexture;
    FFLiMaskTextures        maskTextures;
    FFLVec3                 beardPos;
    FFLVec3                 hairPos;
    FFLVec3                 faceCenterPos;  // Used to calculate nose(line) and glass position
    FFLPartsTransform       partsTransform;
    FFLModelType            modelType;
    FFLBoundingBox          boundingBox[3];
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
