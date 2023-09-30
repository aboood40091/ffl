#ifndef FFLI_RENDER_TEXTURE_H_
#define FFLI_RENDER_TEXTURE_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

struct FFLiRenderTexture
{
    GX2Texture  gx2Texture;
};
NN_STATIC_ASSERT(sizeof(FFLiRenderTexture) == 0x9C);

#endif // FFLI_RENDER_TEXTURE_H_
