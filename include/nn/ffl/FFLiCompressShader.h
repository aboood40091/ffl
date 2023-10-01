#ifndef FFLI_COMPRESS_SHADER_H_
#define FFLI_COMPRESS_SHADER_H_

#include <nn/ffl/FFLiUtilShader.h>

#include <cafe/gx2.h>

class FFLiCompressShader
{
public:
    FFLiCompressShader();
    ~FFLiCompressShader();

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
