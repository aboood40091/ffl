#ifndef FFLI_TEXTURE_TEMP_OBJECT_H_
#define FFLI_TEXTURE_TEMP_OBJECT_H_

#include <nn/ffl/FFLiFacelineTextureTempObject.h>
#include <nn/ffl/FFLiMaskTexturesTempObject.h>

struct FFLiTextureTempObject
{
    FFLiMaskTexturesTempObject      maskTextures;
    FFLiFacelineTextureTempObject   facelineTexture;
};
NN_STATIC_ASSERT(sizeof(FFLiTextureTempObject) == (sizeof(FFLiMaskTexturesTempObject) + sizeof(FFLiFacelineTextureTempObject)));

#endif // FFLI_TEXTURE_TEMP_OBJECT_H_
