#ifndef FFLI_SHAPE_H_
#define FFLI_SHAPE_H_

#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiShapePartsType.h>

struct FFLBoundingBox;
struct FFLDrawParam;
struct FFLVec3;

class   FFLiBufferAllocator;
struct  FFLiCharModel;
class   FFLiCoordinate;
class   FFLiResourceLoader;

bool FFLiCanDrawShape(const FFLDrawParam* pDrawParam);

FFLResult FFLiLoadShape(FFLDrawParam* pDrawParam, FFLBoundingBox* pBoundingBox, FFLiCharModel* pModel, FFLiShapePartsType partsType, u16 index, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator);
void FFLiAdjustShape(FFLDrawParam* pDrawParam, FFLBoundingBox* pBoundingBox, f32 scaleX, f32 scaleY, const FFLVec3* pTranslate, bool flipX, const FFLiCoordinate* pCoordinate, FFLiShapePartsType partsType, bool limitNoseScaleZ);

void FFLiInvalidateShape(FFLDrawParam* pDrawParam);

#endif // FFLI_SHAPE_H_
