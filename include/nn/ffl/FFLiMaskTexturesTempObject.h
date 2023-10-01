#ifndef FFLI_MASK_TEXTURES_TEMP_OBJECT_H_
#define FFLI_MASK_TEXTURES_TEMP_OBJECT_H_

#include <nn/ffl/FFLExpression.h>

#include <nn/ffl/FFLiPartsTextures.h>

class   FFLiCompressorParam;
struct  FFLiRawMaskDrawParam;
struct  FFLiRenderTexture;

struct FFLiMaskTexturesTempObject
{
    FFLiPartsTextures       partsTextures;
    FFLiRawMaskDrawParam*   pRawMaskDrawParam[FFL_EXPRESSION_MAX];
    FFLiRenderTexture*      pRenderTexture;
    FFLiCompressorParam*    pCompressorParam[FFL_EXPRESSION_MAX];
};
NN_STATIC_ASSERT(sizeof(FFLiMaskTexturesTempObject) == 0xD4);

#endif // FFLI_MASK_TEXTURES_TEMP_OBJECT_H_
