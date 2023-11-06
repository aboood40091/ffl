#ifndef FFLI_RAW_MASK_H_
#define FFLI_RAW_MASK_H_

#include <nn/ffl/FFLiRawMaskParts.h>

#include <common/aglTextureData.h>

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
    agl::TextureData*   pTexturesEye[2];
    agl::TextureData*   pTexturesEyebrow[2];
    agl::TextureData*   pTextureMouth;
    agl::TextureData*   pTexturesMustache[2];
    agl::TextureData*   pTextureMole;
};
NN_STATIC_ASSERT(sizeof(FFLiRawMaskTextureDesc) == 0x20);

struct  FFLiCharInfo;
class   FFLiShaderCallback;

void FFLiInitDrawParamRawMask(FFLiRawMaskDrawParam* pDrawParam, const FFLiCharInfo* pCharInfo, s32 resolution, s32 leftEyeIndex, s32 rightEyeIndex, const FFLiRawMaskTextureDesc* pDesc);
void FFLiDeleteDrawParamRawMask(FFLiRawMaskDrawParam* pDrawParam);
void FFLiInvalidateRawMask(FFLiRawMaskDrawParam* pDrawParam);
void FFLiDrawRawMask(const FFLiRawMaskDrawParam* pDrawParam, const FFLiShaderCallback* pCallback);

#endif // FFLI_RAW_MASK_H_
