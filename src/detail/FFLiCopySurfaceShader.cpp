#include <nn/ffl/detail/FFLiCopySurfaceShader.h>

#include "shaders/FFLiCopySurfaceShaderObj.h"

#if RIO_IS_CAFE

static void FFLiInvalidateShader(void* ptr, u32 size)
{
    GX2Invalidate(GX2_INVALIDATE_CPU_SHADER, ptr, size);
}

static void InvalidateVertexShader(GX2VertexShader* pShader)
{
    FFLiInvalidateShader(pShader->shaderPtr, pShader->shaderSize);
}

static void InvalidatePixelShader(GX2PixelShader* pShader)
{
    FFLiInvalidateShader(pShader->shaderPtr, pShader->shaderSize);
}

#endif // RIO_IS_CAFE

FFLiCopySurfaceShader::FFLiCopySurfaceShader()
    : m_SamplerLocation(0)
{
#if RIO_IS_CAFE
    m_Shader.load(&FFLiCopySurfaceShaderObj_VS, &FFLiCopySurfaceShaderObj_PS);
#elif RIO_IS_WIN
    m_Shader.load(FFLiCopySurfaceShaderObj_VS_src, FFLiCopySurfaceShaderObj_PS_src);
#endif
}

FFLiCopySurfaceShader::~FFLiCopySurfaceShader()
{
}

namespace {

const char* TEXTURE_SAMPLER_NAME[1] = {
    "s_texture"
};

}

bool FFLiCopySurfaceShader::SetupCPU()
{
    u32 location = m_Shader.getFragmentSamplerLocation(TEXTURE_SAMPLER_NAME[0]);
    if (location == u32(-1))
        return false;

    m_SamplerLocation = location;

    m_Sampler.setWrap(agl::cTextureWrapType_Mirror, agl::cTextureWrapType_Mirror, agl::cTextureWrapType_Mirror);
  //m_Sampler.setFilterMin(agl::cTextureFilterType_Linear); // <-- Already set
  //m_Sampler.setFilterMag(agl::cTextureFilterType_Linear); // ^^^

    return true;
}

void FFLiCopySurfaceShader::SetupGPU()
{
#if RIO_IS_CAFE
    InvalidateVertexShader(&FFLiCopySurfaceShaderObj_VS);
    InvalidatePixelShader(&FFLiCopySurfaceShaderObj_PS);
#endif // RIO_IS_CAFE
}

void FFLiCopySurfaceShader::Bind() const
{
    m_Shader.bind(true);
}

void FFLiCopySurfaceShader::SetTexture(const agl::TextureData& textureData, u32 mipLevel)
{
    m_Sampler.setMipParam(mipLevel, mipLevel, 0.0f);
    m_Sampler.applyTextureData(textureData);
    m_Sampler.activate(-1, m_SamplerLocation, -1, 0);
}
