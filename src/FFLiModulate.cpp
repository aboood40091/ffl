#include <nn/ffl/FFLModulateParam.h>
#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiModulate.h>

void FFLiInitModulateMustache(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_3;
    pParam->type = FFL_MODULATE_TYPE_MUSTACHE;
    pParam->pColorR = &FFLiGetSrgbFetchMustacheColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateMouth(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_2;
    pParam->type = FFL_MODULATE_TYPE_MOUTH;
    pParam->pColorR = &FFLiGetSrgbFetchMouthColorR(color);
    pParam->pColorG = &FFLiGetSrgbFetchMouthColorG(color);
    pParam->pColorB = &FFLiGetSrgbFetchMouthColorB(color);
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateEyebrow(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_3;
    pParam->type = FFL_MODULATE_TYPE_EYEBROW;
    pParam->pColorR = &FFLiGetSrgbFetchEyebrowColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateEye(FFLModulateParam* pParam, s32 colorGB, s32 colorR, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_2;
    pParam->type = FFL_MODULATE_TYPE_EYE;
    pParam->pColorR = &FFLiGetSrgbFetchEyeColorR(colorR);
    pParam->pColorG = &FFLiGetSrgbFetchEyeColorG(colorGB);
    pParam->pColorB = &FFLiGetSrgbFetchEyeColorB(colorGB);
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateMole(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_3;
    pParam->type = FFL_MODULATE_TYPE_MOLE;
    pParam->pColorR = &FFLiGetSrgbFetchMoleColor();
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateFaceMake(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_1;
    pParam->type = FFL_MODULATE_TYPE_FACE_MAKE;
    pParam->pColorR = NULL;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateFaceLine(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_3;
    pParam->type = FFL_MODULATE_TYPE_FACE_LINE;
    pParam->pColorR = &FFLiGetFaceLine();
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateFaceBeard(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_3;
    pParam->type = FFL_MODULATE_TYPE_FACE_BEARD;
    pParam->pColorR = &FFLiGetSrgbFetchBeardColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeFaceline(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_1;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_FACELINE;
    pParam->pColorR = NULL;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeBeard(FFLModulateParam* pParam, s32 color)
{
    pParam->mode = FFL_MODULATE_MODE_0;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_BEARD;
    pParam->pColorR = &FFLiGetHairColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeNose(FFLModulateParam* pParam, s32 color)
{
    pParam->mode = FFL_MODULATE_MODE_0;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_NOSE;
    pParam->pColorR = &FFLiGetFacelineColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeForehead(FFLModulateParam* pParam, s32 color)
{
    pParam->mode = FFL_MODULATE_MODE_0;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_FOREHEAD;
    pParam->pColorR = &FFLiGetFacelineColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeHair(FFLModulateParam* pParam, s32 color)
{
    pParam->mode = FFL_MODULATE_MODE_0;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_HAIR;
    pParam->pColorR = &FFLiGetHairColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeCap(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_5;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_CAP;
    pParam->pColorR = &FFLiGetCapColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeMask(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_1;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_MASK;
    pParam->pColorR = NULL;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeNoseline(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_3;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_NOSELINE;
    pParam->pColorR = &FFLiGetNoselineColor();
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeGlass(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->mode = FFL_MODULATE_MODE_4;
    pParam->type = FFL_MODULATE_TYPE_SHAPE_GLASS;
    pParam->pColorR = &FFLiGetGlassColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

static const FFLColor FILL_COLOR = {
    0.0f, 0.0f, 0.0f, 0.0f
};

void FFLiInitModulateFill(FFLModulateParam* pParam)
{
    pParam->mode = FFL_MODULATE_MODE_0;
    pParam->type = FFL_MODULATE_TYPE_FILL;
    pParam->pColorR = &FILL_COLOR;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}
