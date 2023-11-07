#ifndef FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_
#define FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_

#include <nn/ffl/FFLDrawParam.h>

#include <common/aglTextureData.h>

struct  FFLiRenderTexture;

struct FFLiFacelineTextureTempObject
{
    agl::TextureData*   pTextureFaceLine;
    FFLDrawParam        drawParamFaceLine;
    agl::TextureData*   pTextureFaceMake;
    FFLDrawParam        drawParamFaceMake;
    agl::TextureData*   pTextureFaceBeard;
    FFLDrawParam        drawParamFaceBeard;
    void*               _144;   // Deleted
    void*               _148;   // Deleted
};
NN_STATIC_ASSERT(sizeof(FFLiFacelineTextureTempObject) == 0x14C);

#endif // FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_
