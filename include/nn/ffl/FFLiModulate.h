#ifndef FFLI_MODULATE_H_
#define FFLI_MODULATE_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

struct FFLModulateParam;

void FFLiInitModulateShapeMask(FFLModulateParam* pParam, const GX2Texture& gx2Texture);

#endif // FFLI_MODULATE_H_
