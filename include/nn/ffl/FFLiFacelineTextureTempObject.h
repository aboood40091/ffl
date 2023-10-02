#ifndef FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_
#define FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_

#include <nn/ffl/FFLDrawParam.h>

#include <cafe/gx2.h>

class   FFLiCompressorParam;
struct  FFLiRenderTexture;

struct FFLiFacelineTextureTempObject
{
    GX2Texture*             pTextureFaceline;
    FFLDrawParam            drawParamFaceline;
    GX2Texture*             pTextureFaceMakeup;
    FFLDrawParam            drawParamFaceMakeup;
    GX2Texture*             pTextureBeard;
    FFLDrawParam            drawParamBeard;
    FFLiRenderTexture*      pRenderTexture;
    FFLiCompressorParam*    pCompressorParam;
};
NN_STATIC_ASSERT(sizeof(FFLiFacelineTextureTempObject) == 0x14C);

#endif // FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_
