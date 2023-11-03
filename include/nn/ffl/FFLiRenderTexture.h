#ifndef FFLI_RENDER_TEXTURE_H_
#define FFLI_RENDER_TEXTURE_H_

#include <nn/ffl/types.h>

#include <common/aglRenderBuffer.h>
#include <common/aglRenderTarget.h>

struct FFLiRenderTexture
{
    agl::TextureData        textureData;

    agl::RenderBuffer       renderBuffer;
    agl::RenderTargetColor  colorTarget;
    agl::RenderTargetDepth  depthTarget;
};
//NN_STATIC_ASSERT(sizeof(FFLiRenderTexture) == 0x9C);

struct FFLColor;

struct  FFLiRenderTextureBuffer;
class   FFLiShaderCallback;

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, rio::TextureFormat format, u32 numMips);
void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture);
void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, agl::TextureData* pDepthBuffer, u32 mipLevel, const FFLiShaderCallback* pCallback);
void FFLiFlushRenderTexture(FFLiRenderTexture* pRenderTexture);

#endif // FFLI_RENDER_TEXTURE_H_
