#ifndef FFLI_PARTS_TEXTURES_H_
#define FFLI_PARTS_TEXTURES_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

struct FFLiPartsTextures
{
    GX2Texture* pTextureEye[6];
    GX2Texture* pTextureMouth[5];
    GX2Texture* pTextureEyebrow;
    GX2Texture* pTextureMustache;
    GX2Texture* pTextureMole;
};
NN_STATIC_ASSERT(sizeof(FFLiPartsTextures) == 0x38);

#endif // FFLI_PARTS_TEXTURES_H_
