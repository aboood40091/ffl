#ifndef FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_
#define FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_

#include <nn/ffl/FFLDrawParam.h>

#include <gpu/rio_Texture.h>

struct  FFLiRenderTexture;

struct FFLiFacelineTextureTempObject
{
    rio::Texture2D* pTextureFaceLine;
    FFLDrawParam    drawParamFaceLine;
    rio::Texture2D* pTextureFaceMake;
    FFLDrawParam    drawParamFaceMake;
    rio::Texture2D* pTextureFaceBeard;
    FFLDrawParam    drawParamFaceBeard;
    void*           _144;   // Deleted
    void*           _148;   // Deleted
};
NN_STATIC_ASSERT(sizeof(FFLiFacelineTextureTempObject) == (3 * (NN_ALIGN_TO(NN_PTR_SIZE, FFL_DRAW_PARAM_ALIGN) + FFL_DRAW_PARAM_SIZE) + 2 * NN_PTR_SIZE));

#endif // FFLI_FACELINE_TEXTURE_TEMP_OBJECT_H_
