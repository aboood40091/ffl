#ifndef FFL_CHAR_MODEL_H_
#define FFL_CHAR_MODEL_H_

#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLModelType.h>
#include <nn/ffl/FFLResult.h>

#ifdef __cplusplus
extern "C" {
#endif

#if RIO_IS_CAFE
#define FFL_CHAR_MODEL_SIZE (0xA70)
#else
#define FFL_CHAR_MODEL_SIZE (0x950)
#endif // RIO_IS_CAFE

typedef struct FFLCharModel
{
    union
    {
        u8 data[FFL_CHAR_MODEL_SIZE];
        u32 data32[FFL_CHAR_MODEL_SIZE / sizeof(u32)];
    };
}
FFLCharModel;
NN_STATIC_ASSERT(sizeof(FFLCharModel) == FFL_CHAR_MODEL_SIZE);

typedef struct FFLCharModelDesc FFLCharModelDesc;
typedef struct FFLCharModelSource FFLCharModelSource;
typedef struct FFLCharModelBuffer FFLCharModelBuffer;
typedef struct FFLDrawParam FFLDrawParam;
typedef struct FFLPartsTransform FFLPartsTransform;
typedef struct FFLShaderCallback FFLShaderCallback;

u32 FFLGetBufferSizeCharModel(const FFLCharModelDesc* pDesc);
u32 FFLGetTempBufferSizeCharModel(const FFLCharModelDesc* pDesc);

FFLResult FFLInitCharModelCPUStep(FFLCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc, FFLCharModelBuffer* pBuffer);

void FFLInitCharModelGPUStep(FFLCharModel* pModel);
void FFLInitCharModelGPUStepWithCallback(FFLCharModel* pModel, const FFLShaderCallback* pCallback);

void FFLDeleteCharModel(FFLCharModel* pModel);

const FFLDrawParam* FFLGetDrawParamOpaFaceline(const FFLCharModel* pModel);
const FFLDrawParam* FFLGetDrawParamOpaBeard(const FFLCharModel* pModel);
const FFLDrawParam* FFLGetDrawParamOpaNose(const FFLCharModel* pModel);
const FFLDrawParam* FFLGetDrawParamOpaForehead(const FFLCharModel* pModel);
const FFLDrawParam* FFLGetDrawParamOpaHair(const FFLCharModel* pModel);
const FFLDrawParam* FFLGetDrawParamOpaCap(const FFLCharModel* pModel);
void FFLDrawOpa(const FFLCharModel* pModel);
void FFLDrawOpaWithCallback(const FFLCharModel* pModel, const FFLShaderCallback* pCallback);

const FFLDrawParam* FFLGetDrawParamXluMask(const FFLCharModel* pModel);
const FFLDrawParam* FFLGetDrawParamXluNoseLine(const FFLCharModel* pModel);
const FFLDrawParam* FFLGetDrawParamXluGlass(const FFLCharModel* pModel);
void FFLDrawXlu(const FFLCharModel* pModel);
void FFLDrawXluWithCallback(const FFLCharModel* pModel, const FFLShaderCallback* pCallback);

void FFLSetExpression(FFLCharModel* pModel, FFLExpression expression);
FFLExpression FFLGetExpression(const FFLCharModel* pModel);

void FFLGetPartsTransform(FFLPartsTransform* pTransform, const FFLCharModel* pModel);

void FFLSetViewModelType(FFLCharModel* pModel, FFLModelType type);

#ifdef __cplusplus
}
#endif

#endif // FFL_CHAR_MODEL_H_
