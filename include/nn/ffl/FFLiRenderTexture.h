#ifndef FFLI_RENDER_TEXTURE_H_
#define FFLI_RENDER_TEXTURE_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

struct FFLiRenderTexture
{
    GX2Texture  gx2Texture;
};
NN_STATIC_ASSERT(sizeof(FFLiRenderTexture) == 0x9C);

u32 FFLiGetBufferRenderTexture(u32 width, u32 height, GX2SurfaceFormat format, u32 numMips);

struct FFLColor;

class   FFLiBufferAllocator;
struct  FFLiRenderTextureBuffer;
class   FFLiShaderCallback;

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, GX2SurfaceFormat format, u32 numMips, FFLiBufferAllocator* pAllocator);
void FFLiInitByBufferRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, GX2SurfaceFormat format, u32 numMips, FFLiRenderTextureBuffer* pRenderTextureBuffer);
void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture);
void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, void* pDepthBuffer, u32 mipLevel, GX2SurfaceFormat format, const FFLiShaderCallback* pCallback);
void FFLiSetupRenderTexture(GX2Texture* pGX2Texture, const FFLColor* pClearColor, void* pDepthBuffer, u32 mipLevel, GX2SurfaceFormat format, const FFLiShaderCallback* pCallback);
void FFLiFlushRenderTexture(FFLiRenderTexture* pRenderTexture);

#endif // FFLI_RENDER_TEXTURE_H_
