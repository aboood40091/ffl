#include <nn/ffl/FFLModulateParam.h>
#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiModulate.h>

void FFLiInitModulateMustache(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->_0 = 3;
    pParam->_4 = 9;
    pParam->pColorR = &FFLiGetSrgbFetchMustacheColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateMouth(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->_0 = 2;
    pParam->_4 = 10;
    pParam->pColorR = &FFLiGetSrgbFetchMouthColorR(color);
    pParam->pColorG = &FFLiGetSrgbFetchMouthColorG(color);
    pParam->pColorB = &FFLiGetSrgbFetchMouthColorB(color);
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateEyebrow(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->_0 = 3;
    pParam->_4 = 11;
    pParam->pColorR = &FFLiGetSrgbFetchEyebrowColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateEye(FFLModulateParam* pParam, s32 colorGB, s32 colorR, const GX2Texture& gx2Texture)
{
    pParam->_0 = 2;
    pParam->_4 = 12;
    pParam->pColorR = &FFLiGetSrgbFetchEyeColorR(colorR);
    pParam->pColorG = &FFLiGetSrgbFetchEyeColorG(colorGB);
    pParam->pColorB = &FFLiGetSrgbFetchEyeColorB(colorGB);
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateMole(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->_0 = 3;
    pParam->_4 = 13;
    pParam->pColorR = &FFLiGetSrgbFetchMoleColor();
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateFaceMake(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->_0 = 1;
    pParam->_4 = 14;
    pParam->pColorR = NULL;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateFaceLine(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->_0 = 3;
    pParam->_4 = 15;
    pParam->pColorR = &FFLiGetFaceLine();
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateFaceBeard(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->_0 = 3;
    pParam->_4 = 16;
    pParam->pColorR = &FFLiGetSrgbFetchBeardColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeFaceline(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->_0 = 1;
    pParam->_4 = 0;
    pParam->pColorR = NULL;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeBeard(FFLModulateParam* pParam, s32 color)
{
    pParam->_0 = 0;
    pParam->_4 = 1;
    pParam->pColorR = &FFLiGetHairColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeNose(FFLModulateParam* pParam, s32 color)
{
    pParam->_0 = 0;
    pParam->_4 = 2;
    pParam->pColorR = &FFLiGetFacelineColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeForehead(FFLModulateParam* pParam, s32 color)
{
    pParam->_0 = 0;
    pParam->_4 = 3;
    pParam->pColorR = &FFLiGetFacelineColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeHair(FFLModulateParam* pParam, s32 color)
{
    pParam->_0 = 0;
    pParam->_4 = 4;
    pParam->pColorR = &FFLiGetHairColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}

void FFLiInitModulateShapeCap(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->_0 = 5;
    pParam->_4 = 5;
    pParam->pColorR = &FFLiGetCapColor(color);
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeMask(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->_0 = 1;
    pParam->_4 = 6;
    pParam->pColorR = NULL;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeNoseline(FFLModulateParam* pParam, const GX2Texture& gx2Texture)
{
    pParam->_0 = 3;
    pParam->_4 = 7;
    pParam->pColorR = &FFLiGetNoselineColor();
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = &gx2Texture;
}

void FFLiInitModulateShapeGlass(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture)
{
    pParam->_0 = 4;
    pParam->_4 = 8;
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
    pParam->_0 = 0;
    pParam->_4 = 17;
    pParam->pColorR = &FILL_COLOR;
    pParam->pColorG = NULL;
    pParam->pColorB = NULL;
    pParam->pGX2Texture = NULL;
}
