#ifndef FFLI_MODULATE_H_
#define FFLI_MODULATE_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

struct FFLModulateParam;

void FFLiInitModulateFaceMake(FFLModulateParam* pParam, const GX2Texture& gx2Texture);
void FFLiInitModulateFaceLine(FFLModulateParam* pParam, const GX2Texture& gx2Texture);
void FFLiInitModulateFaceBeard(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture);

void FFLiInitModulateShapeFaceline(FFLModulateParam* pParam, const GX2Texture& gx2Texture);
void FFLiInitModulateShapeBeard(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeNose(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeForehead(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeHair(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeCap(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture);
void FFLiInitModulateShapeMask(FFLModulateParam* pParam, const GX2Texture& gx2Texture);
void FFLiInitModulateShapeNoseline(FFLModulateParam* pParam, const GX2Texture& gx2Texture);
void FFLiInitModulateShapeGlass(FFLModulateParam* pParam, s32 color, const GX2Texture& gx2Texture);

#endif // FFLI_MODULATE_H_
