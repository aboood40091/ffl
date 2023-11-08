#ifndef FFLI_MODULATE_H_
#define FFLI_MODULATE_H_

#include <nn/ffl/types.h>

#include <gpu/rio_Texture.h>

struct FFLModulateParam;

void FFLiInitModulateMustache(FFLModulateParam* pParam, s32 color, const rio::Texture2D& texture2D);
void FFLiInitModulateMouth(FFLModulateParam* pParam, s32 color, const rio::Texture2D& texture2D);
void FFLiInitModulateEyebrow(FFLModulateParam* pParam, s32 color, const rio::Texture2D& texture2D);
void FFLiInitModulateEye(FFLModulateParam* pParam, s32 colorGB, s32 colorR, const rio::Texture2D& texture2D);
void FFLiInitModulateMole(FFLModulateParam* pParam, const rio::Texture2D& texture2D);
void FFLiInitModulateFaceMake(FFLModulateParam* pParam, const rio::Texture2D& texture2D);
void FFLiInitModulateFaceLine(FFLModulateParam* pParam, const rio::Texture2D& texture2D);
void FFLiInitModulateFaceBeard(FFLModulateParam* pParam, s32 color, const rio::Texture2D& texture2D);

void FFLiInitModulateShapeFaceline(FFLModulateParam* pParam, const rio::Texture2D& texture2D);
void FFLiInitModulateShapeBeard(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeNose(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeForehead(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeHair(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeCap(FFLModulateParam* pParam, s32 color, const rio::Texture2D& texture2D);
void FFLiInitModulateShapeMask(FFLModulateParam* pParam, const rio::Texture2D& texture2D);
void FFLiInitModulateShapeNoseline(FFLModulateParam* pParam, const rio::Texture2D& texture2D);
void FFLiInitModulateShapeGlass(FFLModulateParam* pParam, s32 color, const rio::Texture2D& texture2D);

void FFLiInitModulateFill(FFLModulateParam* pParam);

#endif // FFLI_MODULATE_H_
