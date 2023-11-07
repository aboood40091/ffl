#ifndef FFLI_COPY_SURFACE_SHADER_H_
#define FFLI_COPY_SURFACE_SHADER_H_

#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <nn/ffl/FFLiUtilShader.h>

#include <gx2/texture.h>

class FFLiCopySurfaceShader
{
public:
    FFLiCopySurfaceShader();
    ~FFLiCopySurfaceShader();

    bool SetupCPU();
    void SetupGPU();

    void Bind() const;

    void SetTexture(const GX2Texture* pGX2Texture, u32 mipLevel);

private:
    FFLiUtilShader::FetchShader m_FetchShader;
    GX2VertexShader*            m_pVertexShader;
    GX2PixelShader*             m_pPixelShader;
    GX2Sampler                  m_Sampler;
    u32                         m_SamplerLocation;
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurfaceShader) == 0x3C);

#endif // RIO_IS_CAFE

#endif // FFLI_COPY_SURFACE_SHADER_H_
