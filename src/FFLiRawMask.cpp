#include <nn/ffl/FFLiModulate.h>
#include <nn/ffl/FFLiRawMask.h>
#include <nn/ffl/FFLiRawMaskParts.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

namespace {

enum
{
    RAW_MASK_TYPE_MAX = sizeof(FFLiRawMaskDrawParam) / sizeof(FFLiRawMaskPartsDrawParam)
};

struct RawMasks
{
    FFLiRawMaskPartsDesc    rawMaskPartsDescEye[2];
    FFLiRawMaskPartsDesc    rawMaskPartsDescEyebrow[2];
    FFLiRawMaskPartsDesc    rawMaskPartsDescMouth;
    FFLiRawMaskPartsDesc    rawMaskPartsDescMustache[2];
    FFLiRawMaskPartsDesc    rawMaskPartsDescMole;
};

void CalcRawMask(RawMasks* pRawMasks, const FFLiCharInfo* pCharInfo, s32 resolution, s32 leftEyeIndex, s32 rightEyeIndex);

}

u32 FFLiGetBufferRawMask()
{
    return FFLiGetBufferRawMaskParts() * RAW_MASK_TYPE_MAX;
}

void FFLiInitDrawParamRawMask(FFLiRawMaskDrawParam* pDrawParam, const FFLiCharInfo* pCharInfo, s32 resolution, s32 leftEyeIndex, s32 rightEyeIndex, const FFLiRawMaskTextureDesc* pDesc, FFLiBufferAllocator* pAllocator)
{
    Mat44 projMatrix;
    RawMasks rawMasks;

    CalcRawMask(&rawMasks, pCharInfo, resolution, leftEyeIndex, rightEyeIndex);

    MTXOrtho(projMatrix.mtx, 0.0f, resolution, 0.0f, resolution, -200.0f, 200.0f);

    FFLiInitModulateMustache(&pDrawParam->drawParamRawMaskPartsMustache[0].modulateParam, pCharInfo->parts.beardColor, *(pDesc->pTexturesMustache[0]));
    FFLiInitDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[0]), &(rawMasks.rawMaskPartsDescMustache[0]), &projMatrix, pAllocator);

    FFLiInitModulateMustache(&pDrawParam->drawParamRawMaskPartsMustache[1].modulateParam, pCharInfo->parts.beardColor, *(pDesc->pTexturesMustache[1]));
    FFLiInitDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[1]), &(rawMasks.rawMaskPartsDescMustache[1]), &projMatrix, pAllocator);
    
    FFLiInitModulateMouth(&pDrawParam->drawParamRawMaskPartsMouth.modulateParam, pCharInfo->parts.mouthColor, *pDesc->pTextureMouth);
    FFLiInitDrawParamRawMaskParts(&pDrawParam->drawParamRawMaskPartsMouth, &rawMasks.rawMaskPartsDescMouth, &projMatrix, pAllocator);
    
    FFLiInitModulateEyebrow(&pDrawParam->drawParamRawMaskPartsEyebrow[0].modulateParam, pCharInfo->parts.eyebrowColor, *(pDesc->pTexturesEyebrow[0]));
    FFLiInitDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[0]), &(rawMasks.rawMaskPartsDescEyebrow[0]), &projMatrix, pAllocator);
    
    FFLiInitModulateEyebrow(&pDrawParam->drawParamRawMaskPartsEyebrow[1].modulateParam, pCharInfo->parts.eyebrowColor, *(pDesc->pTexturesEyebrow[1]));
    FFLiInitDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[1]), &(rawMasks.rawMaskPartsDescEyebrow[1]), &projMatrix, pAllocator);
    
    FFLiInitModulateEye(&pDrawParam->drawParamRawMaskPartsEye[0].modulateParam, pCharInfo->parts.eyeColor, pCharInfo->parts.eyeType, *(pDesc->pTexturesEye[0]));
    FFLiInitDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[0]), &(rawMasks.rawMaskPartsDescEye[0]), &projMatrix, pAllocator);
    
    FFLiInitModulateEye(&pDrawParam->drawParamRawMaskPartsEye[1].modulateParam, pCharInfo->parts.eyeColor, pCharInfo->parts.eyeType, *(pDesc->pTexturesEye[1]));
    FFLiInitDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[1]), &(rawMasks.rawMaskPartsDescEye[1]), &projMatrix, pAllocator);
    
    FFLiInitModulateMole(&pDrawParam->drawParamRawMaskPartsMole.modulateParam, *pDesc->pTextureMole);
    FFLiInitDrawParamRawMaskParts(&pDrawParam->drawParamRawMaskPartsMole, &rawMasks.rawMaskPartsDescMole, &projMatrix, pAllocator);
    
    FFLiInitModulateFill(&pDrawParam->drawParamRawMaskPartsFill.modulateParam);
    FFLiInitDrawParamRawMaskPartsFill(&pDrawParam->drawParamRawMaskPartsFill, pAllocator);
}

void FFLiInvalidateRawMask(FFLiRawMaskDrawParam* pDrawParam)
{
    FFLiInvalidateDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[0]));
    FFLiInvalidateDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[1]));
    FFLiInvalidateDrawParamRawMaskParts(&pDrawParam->drawParamRawMaskPartsMouth);
    FFLiInvalidateDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[0]));
    FFLiInvalidateDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[1]));
    FFLiInvalidateDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[0]));
    FFLiInvalidateDrawParamRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[1]));
    FFLiInvalidateDrawParamRawMaskParts(&pDrawParam->drawParamRawMaskPartsMole);
    FFLiInvalidateDrawParamRawMaskParts(&pDrawParam->drawParamRawMaskPartsFill);
}

void FFLiDrawRawMask(const FFLiRawMaskDrawParam* pDrawParam, const FFLiShaderCallback* pCallback)
{
    GX2SetAlphaTest(GX2_ENABLE, GX2_COMPARE_GREATER, 0.0f);
    GX2SetColorControl(GX2_LOGIC_OP_NONE, 0xFF, GX2_DISABLE, GX2_ENABLE);
    GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_LESS);
    GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, GX2_DISABLE, GX2_DISABLE);
    GX2SetBlendControl(
        GX2_RENDER_TARGET_0,
        
        GX2_BLEND_ONE_MINUS_DST_ALPHA,
        GX2_BLEND_DST_ALPHA,
        GX2_BLEND_COMBINE_ADD,
        
        GX2_ENABLE,
        
        GX2_BLEND_ONE,
        GX2_BLEND_ONE,
        GX2_BLEND_COMBINE_MAX
    );

    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[0]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[1]), pCallback);
    FFLiDrawRawMaskParts(&pDrawParam->drawParamRawMaskPartsMouth, pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[0]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[1]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[0]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[1]), pCallback);
    FFLiDrawRawMaskParts(&pDrawParam->drawParamRawMaskPartsMole, pCallback);

    GX2SetTargetChannelMasks(
        GX2_CHANNEL_MASK_A,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE
    );
    GX2SetBlendControl(
        GX2_RENDER_TARGET_0,
        
        GX2_BLEND_ZERO,
        GX2_BLEND_ZERO,
        GX2_BLEND_COMBINE_ADD,

        GX2_DISABLE,

        GX2_BLEND_ZERO,
        GX2_BLEND_ZERO,
        GX2_BLEND_COMBINE_ADD
    );
    GX2SetAlphaTest(GX2_DISABLE, GX2_COMPARE_ALWAYS, 0.0f);

    FFLiDrawRawMaskParts(&pDrawParam->drawParamRawMaskPartsFill, pCallback);

    GX2SetBlendControl(
        GX2_RENDER_TARGET_0,
        
        GX2_BLEND_SRC_ALPHA,
        GX2_BLEND_ONE,
        GX2_BLEND_COMBINE_ADD,

        GX2_DISABLE,

        GX2_BLEND_ONE,
        GX2_BLEND_ONE,
        GX2_BLEND_COMBINE_ADD
    );
    GX2SetAlphaTest(GX2_ENABLE, GX2_COMPARE_GREATER, 0.0f);

    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[0]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsMustache[1]), pCallback);
    FFLiDrawRawMaskParts(&pDrawParam->drawParamRawMaskPartsMouth, pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[0]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEyebrow[1]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[0]), pCallback);
    FFLiDrawRawMaskParts(&(pDrawParam->drawParamRawMaskPartsEye[1]), pCallback);
    FFLiDrawRawMaskParts(&pDrawParam->drawParamRawMaskPartsMole, pCallback);

    GX2SetTargetChannelMasks(
        GX2_CHANNEL_MASK_RGBA,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE
    );
    GX2SetAlphaTest(GX2_DISABLE, GX2_COMPARE_GREATER, 0.0f);
}

namespace {

void CalcRawMask(RawMasks* pRawMasks, const FFLiCharInfo* pCharInfo, s32 resolution, s32 leftEyeIndex, s32 rightEyeIndex)
{
    static f32 POS_X_ADD    = 3.5323312f;
    static f32 POS_Y_ADD    = 4.629278f;

    static f32 SPACING_MUL  = 0.88961464f;
    static f32 POS_X_MUL    = 1.7792293f;
    static f32 POS_Y_MUL    = 1.0760943f;

    static f32 POS_Y_ADD_EYE        = POS_Y_ADD + 13.822246f;
    static f32 POS_Y_ADD_EYEBROW    = POS_Y_ADD + 11.920528f;
    static f32 POS_Y_ADD_MOUTH      = POS_Y_ADD + 24.629572f;
    static f32 POS_Y_ADD_MUSTACHE   = POS_Y_ADD + 27.134275f;
    static f32 POS_X_ADD_MOLE       = POS_X_ADD + 14.233834f;
    static f32 POS_Y_ADD_MOLE       = POS_Y_ADD + 11.178394f + 2 * POS_Y_MUL;

    f32 baseScale = resolution * (1.f / 64.f);

    f32 eyeSpacingX = pCharInfo->parts.eyeSpacingX * SPACING_MUL;
    
    f32 eyeBaseScale = 0.4f * pCharInfo->parts.eyeScale + 1.0f;
    f32 eyeBaseScaleY = 0.12f * pCharInfo->parts.eyeScaleY + 0.64f;
    f32 eyeScaleX = 5.34375f * eyeBaseScale;
    f32 eyeScaleY = 4.5f * eyeBaseScale * eyeBaseScaleY;

    f32 eyePosY = pCharInfo->parts.eyePositionY * POS_Y_MUL + POS_Y_ADD_EYE;

    s32 eyeBaseRotate = pCharInfo->parts.eyeRotate + FFLiiGetEyeRotateOffset(pCharInfo->parts.eyeType);
    f32 eyeRotate = (eyeBaseRotate % 32) * (360.f / 32.f);

    f32 eyebrowSpacingX = pCharInfo->parts.eyebrowSpacingX * SPACING_MUL;
    
    f32 eyebrowBaseScale = 0.4f * pCharInfo->parts.eyebrowScale + 1.0f;
    f32 eyebrowBaseScaleY = 0.12f * pCharInfo->parts.eyebrowScaleY + 0.64f;
    f32 eyebrowScaleX = 5.0625f * eyebrowBaseScale;
    f32 eyebrowScaleY = 4.5f * eyebrowBaseScale * eyebrowBaseScaleY;

    f32 eyebrowPosY = pCharInfo->parts.eyebrowPositionY * POS_Y_MUL + POS_Y_ADD_EYEBROW;

    s32 eyebrowBaseRotate = pCharInfo->parts.eyebrowRotate + FFLiiGetEyebrowRotateOffset(pCharInfo->parts.eyebrowType);
    f32 eyebrowRotate = (eyebrowBaseRotate % 32) * (360.0f / 32);

    f32 mouthBaseScale = 0.4f * pCharInfo->parts.mouthScale + 1.0f;
    f32 mouthBaseScaleY = 0.12f * pCharInfo->parts.mouthScaleY + 0.64f;
    f32 mouthScaleX = 6.1875f * mouthBaseScale;
    f32 mouthScaleY = 4.5f * mouthBaseScale * mouthBaseScaleY;

    f32 mouthPosY = pCharInfo->parts.mouthPositionY * POS_Y_MUL + POS_Y_ADD_MOUTH;

    f32 mustacheBaseScale = 0.4f * pCharInfo->parts.mustacheScale + 1.0f;
    f32 mustacheScaleX = 4.5f * mustacheBaseScale;
    f32 mustacheScaleY = 9.0f * mustacheBaseScale;

    f32 mustachePosY = pCharInfo->parts.mustachePositionY * POS_Y_MUL + POS_Y_ADD_MUSTACHE;

    f32 moleScale = 0.4f * pCharInfo->parts.moleScale + 1.0f;

    f32 molePosX = pCharInfo->parts.molePositionX * POS_X_MUL + POS_X_ADD_MOLE;
    f32 molePosY = pCharInfo->parts.molePositionY * POS_Y_MUL + POS_Y_ADD_MOLE;

    pRawMasks->rawMaskPartsDescEye[0].pos.x = (32 - eyeSpacingX) * baseScale;
    pRawMasks->rawMaskPartsDescEye[0].pos.y = eyePosY * baseScale;
    pRawMasks->rawMaskPartsDescEye[0].scale.x = eyeScaleX * baseScale;
    pRawMasks->rawMaskPartsDescEye[0].scale.y = FFLiiGetAdjustedEyeH(eyeScaleY * baseScale, leftEyeIndex);
    pRawMasks->rawMaskPartsDescEye[0].rot = eyeRotate;
    pRawMasks->rawMaskPartsDescEye[0].originPos = FFLI_ORIGIN_POSITION_LEFT;

    pRawMasks->rawMaskPartsDescEye[1].pos.x = (eyeSpacingX + 32) * baseScale;
    pRawMasks->rawMaskPartsDescEye[1].pos.y = eyePosY * baseScale;
    pRawMasks->rawMaskPartsDescEye[1].scale.x = eyeScaleX * baseScale;
    pRawMasks->rawMaskPartsDescEye[1].scale.y = FFLiiGetAdjustedEyeH(eyeScaleY * baseScale, rightEyeIndex);
    pRawMasks->rawMaskPartsDescEye[1].rot = 360.0f - eyeRotate;
    pRawMasks->rawMaskPartsDescEye[1].originPos = FFLI_ORIGIN_POSITION_RIGHT;

    pRawMasks->rawMaskPartsDescEyebrow[0].pos.x = (32 - eyebrowSpacingX) * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[0].pos.y = eyebrowPosY * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[0].scale.x = eyebrowScaleX * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[0].scale.y = eyebrowScaleY * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[0].rot = eyebrowRotate;
    pRawMasks->rawMaskPartsDescEyebrow[0].originPos = FFLI_ORIGIN_POSITION_LEFT;

    pRawMasks->rawMaskPartsDescEyebrow[1].pos.x = (eyebrowSpacingX + 32) * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[1].pos.y = eyebrowPosY * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[1].scale.x = eyebrowScaleX * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[1].scale.y = eyebrowScaleY * baseScale;
    pRawMasks->rawMaskPartsDescEyebrow[1].rot = 360.0f - eyebrowRotate;
    pRawMasks->rawMaskPartsDescEyebrow[1].originPos = FFLI_ORIGIN_POSITION_RIGHT;

    pRawMasks->rawMaskPartsDescMouth.pos.x = 32 * baseScale;
    pRawMasks->rawMaskPartsDescMouth.pos.y = mouthPosY * baseScale;
    pRawMasks->rawMaskPartsDescMouth.scale.x = mouthScaleX * baseScale;
    pRawMasks->rawMaskPartsDescMouth.scale.y = FFLiiGetAdjustedMouthH(mouthScaleY * baseScale, pCharInfo->parts.mouthType);
    pRawMasks->rawMaskPartsDescMouth.rot = 0.0f;
    pRawMasks->rawMaskPartsDescMouth.originPos = FFLI_ORIGIN_POSITION_CENTER;

    pRawMasks->rawMaskPartsDescMustache[0].pos.x = 32 * baseScale;
    pRawMasks->rawMaskPartsDescMustache[0].pos.y = mustachePosY * baseScale;
    pRawMasks->rawMaskPartsDescMustache[0].scale.x = mustacheScaleX * baseScale;
    pRawMasks->rawMaskPartsDescMustache[0].scale.y = mustacheScaleY * baseScale;
    pRawMasks->rawMaskPartsDescMustache[0].rot = 0.0f;
    pRawMasks->rawMaskPartsDescMustache[0].originPos = FFLI_ORIGIN_POSITION_LEFT;

    pRawMasks->rawMaskPartsDescMustache[1].pos.x = 32 * baseScale;
    pRawMasks->rawMaskPartsDescMustache[1].pos.y = mustachePosY * baseScale;
    pRawMasks->rawMaskPartsDescMustache[1].scale.x = mustacheScaleX * baseScale;
    pRawMasks->rawMaskPartsDescMustache[1].scale.y = mustacheScaleY * baseScale;
    pRawMasks->rawMaskPartsDescMustache[1].rot = 0.0f;
    pRawMasks->rawMaskPartsDescMustache[1].originPos = FFLI_ORIGIN_POSITION_RIGHT;

    pRawMasks->rawMaskPartsDescMole.pos.x = molePosX * baseScale;
    pRawMasks->rawMaskPartsDescMole.pos.y = molePosY * baseScale;
    pRawMasks->rawMaskPartsDescMole.scale.x = moleScale * baseScale;
    pRawMasks->rawMaskPartsDescMole.scale.y = moleScale * baseScale;
    pRawMasks->rawMaskPartsDescMole.rot = 0.0f;
    pRawMasks->rawMaskPartsDescMole.originPos = FFLI_ORIGIN_POSITION_CENTER;
}

}
