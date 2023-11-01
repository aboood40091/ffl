#ifndef FFLI_MODULATE_H_
#define FFLI_MODULATE_H_

#include <nn/ffl/types.h>

#include <common/aglTextureData.h>

struct FFLModulateParam;

void FFLiInitModulateMustache(FFLModulateParam* pParam, s32 color, const agl::TextureData& textureData);
void FFLiInitModulateMouth(FFLModulateParam* pParam, s32 color, const agl::TextureData& textureData);
void FFLiInitModulateEyebrow(FFLModulateParam* pParam, s32 color, const agl::TextureData& textureData);
void FFLiInitModulateEye(FFLModulateParam* pParam, s32 colorGB, s32 colorR, const agl::TextureData& textureData);
void FFLiInitModulateMole(FFLModulateParam* pParam, const agl::TextureData& textureData);
void FFLiInitModulateFaceMake(FFLModulateParam* pParam, const agl::TextureData& textureData);
void FFLiInitModulateFaceLine(FFLModulateParam* pParam, const agl::TextureData& textureData);
void FFLiInitModulateFaceBeard(FFLModulateParam* pParam, s32 color, const agl::TextureData& textureData);

void FFLiInitModulateShapeFaceline(FFLModulateParam* pParam, const agl::TextureData& textureData);
void FFLiInitModulateShapeBeard(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeNose(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeForehead(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeHair(FFLModulateParam* pParam, s32 color);
void FFLiInitModulateShapeCap(FFLModulateParam* pParam, s32 color, const agl::TextureData& textureData);
void FFLiInitModulateShapeMask(FFLModulateParam* pParam, const agl::TextureData& textureData);
void FFLiInitModulateShapeNoseline(FFLModulateParam* pParam, const agl::TextureData& textureData);
void FFLiInitModulateShapeGlass(FFLModulateParam* pParam, s32 color, const agl::TextureData& textureData);

void FFLiInitModulateFill(FFLModulateParam* pParam);

#endif // FFLI_MODULATE_H_
