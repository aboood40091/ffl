#ifndef FFLI_RENDER_TEXTURE_H_
#define FFLI_RENDER_TEXTURE_H_

#include <nn/ffl/types.h>

#include <common/aglRenderBuffer.h>
#include <common/aglRenderTarget.h>

struct FFLiRenderTexture
{
    agl::TextureData*       pTextureData;

    agl::RenderBuffer*      pRenderBuffer;
    agl::RenderTargetColor* pColorTarget;
    agl::RenderTargetDepth* pDepthTarget;
};
NN_STATIC_ASSERT(sizeof(FFLiRenderTexture) == 0x10);

struct FFLColor;

struct  FFLiRenderTextureBuffer;
class   FFLiShaderCallback;

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, rio::TextureFormat format, u32 numMips);
void FFLiDeleteRenderTexture(FFLiRenderTexture* pRenderTexture);
void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture);
void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, agl::TextureData* pDepthBuffer, u32 mipLevel, const FFLiShaderCallback* pCallback);
void FFLiFlushRenderTexture(FFLiRenderTexture* pRenderTexture);

#endif // FFLI_RENDER_TEXTURE_H_
