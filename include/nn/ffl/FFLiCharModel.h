#ifndef FFLI_CHAR_MODEL_H_
#define FFLI_CHAR_MODEL_H_

#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLResult.h>

class FFLiCharModel;

struct FFLCharModelDesc;
struct FFLCharModelSource;
struct FFLCharModelBuffer;
struct FFLDrawParam;
struct FFLPartsTransform;
struct FFLShaderCallback;

u32 FFLiGetBufferSizeCharModel(const FFLCharModelDesc* pDesc);
u32 FFLiGetTempBufferSizeCharModel(const FFLCharModelDesc* pDesc);

FFLResult FFLiInitCharModelCPUStep(FFLiCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc, FFLCharModelBuffer* pBuffer);

void FFLiInitCharModelGPUStep(FFLiCharModel* pModel, const FFLShaderCallback* pCallback);

void FFLiDeleteCharModel(FFLiCharModel* pModel);

const FFLDrawParam* FFLiGetDrawParamOpaFacelineFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaBeardFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaNoseFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaForeheadFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaHairFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamOpaCapFromCharModel(const FFLiCharModel* pModel);

const FFLDrawParam* FFLiGetDrawParamXluMaskFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamXluNoseLineFromCharModel(const FFLiCharModel* pModel);
const FFLDrawParam* FFLiGetDrawParamXluGlassFromCharModel(const FFLiCharModel* pModel);

void FFLiSetExpression(FFLiCharModel* pModel, FFLExpression expression);
FFLExpression FFLiGetExpression(const FFLiCharModel* pModel);

void FFLiGetPartsTransform(FFLPartsTransform* pTransform, const FFLiCharModel* pModel);

void FFLiSetViewModelType(FFLiCharModel* pModel, FFLModelType type);

#endif // FFLI_CHAR_MODEL_H_
