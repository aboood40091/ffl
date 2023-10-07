#ifndef FFLI_RAW_MASK_H_
#define FFLI_RAW_MASK_H_

#include <nn/ffl/FFLiRawMaskParts.h>

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
    GX2Texture* pTexturesEye[2];
    GX2Texture* pTexturesEyebrow[2];
    GX2Texture* pTextureMouth;
    GX2Texture* pTexturesMustache[2];
    GX2Texture* pTextureMole;
};
NN_STATIC_ASSERT(sizeof(FFLiRawMaskTextureDesc) == 0x20);

u32 FFLiGetBufferRawMask();

class FFLiBufferAllocator;
class FFLiShaderCallback;

void FFLiInitDrawParamRawMask(FFLiRawMaskDrawParam* pDrawParamRawMask, const FFLiCharInfo* pCharInfo, s32 resolution, s32 eyeIndex0, s32 eyeIndex1, const FFLiRawMaskTextureDesc* pDesc, FFLiBufferAllocator* pAllocator);
void FFLiInvalidateRawMask(FFLiRawMaskDrawParam* pDrawParamRawMask);
void FFLiDrawRawMask(const FFLiRawMaskDrawParam* pDrawParamRawMask, const FFLiShaderCallback* pCallback);

#endif // FFLI_RAW_MASK_H_
