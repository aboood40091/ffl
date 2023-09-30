#ifndef FFLI_MASK_TEXTURES_H_
#define FFLI_MASK_TEXTURES_H_

#include <nn/ffl/types.h>

struct FFLiRenderTexture;

struct FFLiMaskTextures
{
    FFLiRenderTexture*  renderTextures[19];
};
NN_STATIC_ASSERT(sizeof(FFLiMaskTextures) == 0x4C);

#endif // FFLI_MASK_TEXTURES_H_
