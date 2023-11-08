#ifndef FFLI_RENDER_TEXTURE_H_
#define FFLI_RENDER_TEXTURE_H_

#include <nn/ffl/types.h>

#include <gpu/rio_RenderBuffer.h>
#include <gpu/rio_RenderTarget.h>

struct FFLiRenderTexture
{
    rio::Texture2D*         pTexture2D;

    rio::RenderBuffer*      pRenderBuffer;
    rio::RenderTargetColor* pColorTarget;
    rio::RenderTargetDepth* pDepthTarget;
};
NN_STATIC_ASSERT(sizeof(FFLiRenderTexture) == 0x10);

struct FFLColor;

class   FFLiShaderCallback;

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, rio::TextureFormat format, u32 numMips);
void FFLiDeleteRenderTexture(FFLiRenderTexture* pRenderTexture);
void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture);
void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, rio::Texture2D* pDepthBuffer, u32 mipLevel, const FFLiShaderCallback* pCallback);
void FFLiFlushRenderTexture(FFLiRenderTexture* pRenderTexture);

#endif // FFLI_RENDER_TEXTURE_H_
