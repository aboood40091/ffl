#ifndef FFLI_COMPRESS_SHADER_H_
#define FFLI_COMPRESS_SHADER_H_

#include <nn/ffl/FFLiUtilShader.h>

#include <cafe/gx2.h>

class FFLiBufferAllocator;
class FFLiCompressUniform;

class FFLiCompressShader
{
public:
    FFLiCompressShader();
    ~FFLiCompressShader();  // Deleted in NSMBU

    bool UseUB() const;

    bool SetupLocation();
    bool SetupLocationForUR();
    bool SetupLocationForUB();

    bool SetupCPU(const void* pShaderData, u32 shaderIndex, FFLiBufferAllocator* pAllocator);
    void SetupGPU();

    void Bind() const;

    void SetTexture(const GX2Texture* pGX2Texture) const;

    void SetUniform(const FFLiCompressUniform* pUniform) const;
    void SetUniformUR(const FFLiCompressUniform* pUniform) const;
    void SetUniformUB(const FFLiCompressUniform* pUniform) const;

    static u32 GetBufferSize(const void* pShaderData, u32 shaderIndex);

private:
    FFLiUtilShader::FetchShader m_FetchShader;
    GX2VertexShader*            m_pVertexShader;
    GX2PixelShader*             m_pPixelShader;
    GX2Sampler                  m_Sampler;
    u32                         m_VertexUniformLocation;
    u32                         m_PixelUniformLocation;
    u32                         m_SamplerLocation;
};
NN_STATIC_ASSERT(sizeof(FFLiCompressShader) == 0x44);

#endif // FFLI_COMPRESS_SHADER_H_
