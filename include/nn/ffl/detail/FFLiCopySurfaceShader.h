#ifndef FFLI_COPY_SURFACE_SHADER_H_
#define FFLI_COPY_SURFACE_SHADER_H_

#include <nn/ffl/FFLiUtilShader.h>

class FFLiBufferAllocator;

class FFLiCopySurfaceShader
{
public:
    FFLiCopySurfaceShader();
    ~FFLiCopySurfaceShader();

    void SetupCPU(FFLiBufferAllocator* pAllocator);
    void SetupGPU();

    void Bind() const;

    void SetTexture(const GX2Texture* pGX2Texture, u32 mipLevel) const;

    static u32 GetBufferSize();

private:
    FFLiUtilShader::FetchShader m_FetchShader;
    GX2VertexShader*            m_pVertexShader;
    GX2PixelShader*             m_pPixelShader;
    GX2Sampler                  m_Sampler;
    u32                         m_SamplerLocation;
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurfaceShader) == 0x3C);

#endif // FFLI_COPY_SURFACE_SHADER_H_
