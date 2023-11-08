#ifndef FFLI_RAW_MASK_H_
#define FFLI_RAW_MASK_H_

#include <nn/ffl/FFLiRawMaskParts.h>

#include <gpu/rio_Texture.h>

struct FFLiRawMaskDrawParam
{
    FFLiRawMaskPartsDrawParam   drawParamRawMaskPartsEye[2];
    FFLiRawMaskPartsDrawParam   drawParamRawMaskPartsEyebrow[2];
    FFLiRawMaskPartsDrawParam   drawParamRawMaskPartsMouth;
    FFLiRawMaskPartsDrawParam   drawParamRawMaskPartsMustache[2];
    FFLiRawMaskPartsDrawParam   drawParamRawMaskPartsMole;
    FFLiRawMaskPartsDrawParam   drawParamRawMaskPartsFill;
};
NN_STATIC_ASSERT(sizeof(FFLiRawMaskDrawParam) == 0x3A8);

struct FFLiRawMaskTextureDesc
{
    rio::Texture2D* pTexturesEye[2];
    rio::Texture2D* pTexturesEyebrow[2];
    rio::Texture2D* pTextureMouth;
    rio::Texture2D* pTexturesMustache[2];
    rio::Texture2D* pTextureMole;
};
NN_STATIC_ASSERT(sizeof(FFLiRawMaskTextureDesc) == 0x20);

struct  FFLiCharInfo;
class   FFLiShaderCallback;

void FFLiInitDrawParamRawMask(FFLiRawMaskDrawParam* pDrawParam, const FFLiCharInfo* pCharInfo, s32 resolution, s32 leftEyeIndex, s32 rightEyeIndex, const FFLiRawMaskTextureDesc* pDesc);
void FFLiDeleteDrawParamRawMask(FFLiRawMaskDrawParam* pDrawParam);
void FFLiInvalidateRawMask(FFLiRawMaskDrawParam* pDrawParam);
void FFLiDrawRawMask(const FFLiRawMaskDrawParam* pDrawParam, const FFLiShaderCallback* pCallback);

#endif // FFLI_RAW_MASK_H_
