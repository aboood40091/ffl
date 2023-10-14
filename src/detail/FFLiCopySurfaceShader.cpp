#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiUtilShaderAllocator.h>

#include <nn/ffl/detail/FFLiCopySurfaceShader.h>

#include "shaders/FFLiCopySurfaceShaderObj.h"

static void InvalidateFetchShader(GX2FetchShader* pShader)
{
    FFLiInvalidateShader(pShader->shaderPtr, pShader->shaderSize);
}

static void InvalidateVertexShader(GX2VertexShader* pShader)
{
    FFLiInvalidateShader(pShader->shaderPtr, pShader->shaderSize);
}

static void InvalidatePixelShader(GX2PixelShader* pShader)
{
    FFLiInvalidateShader(pShader->shaderPtr, pShader->shaderSize);
}

FFLiCopySurfaceShader::FFLiCopySurfaceShader()
    : m_FetchShader()
    , m_pVertexShader(&FFLiCopySurfaceShaderObj_VS)
    , m_pPixelShader(&FFLiCopySurfaceShaderObj_PS)
    , m_Sampler()
    , m_SamplerLocation(0)
{
}

FFLiCopySurfaceShader::~FFLiCopySurfaceShader()
{
}

namespace {

const char* TEXTURE_SAMPLER_NAME[1] = {
    "s_texture"
};

FFLiUtilShader::AttributeStreamDesc FETCH_SHADER_DESC[2] = {
    { "a_position", 0, 0, GX2_ATTRIB_FORMAT_32_32_FLOAT },
    { "a_texCoord", 1, 0, GX2_ATTRIB_FORMAT_32_32_FLOAT }
};

}

bool FFLiCopySurfaceShader::SetupCPU(FFLiBufferAllocator* pAllocator)
{
    FFLiUtilShaderAllocator shaderAllocator(pAllocator, 2);

    if (!FFLiUtilShader::BuildShader(&m_FetchShader, m_pVertexShader, &shaderAllocator, FETCH_SHADER_DESC, 2, false))
        return false;

    if (!FFLiUtilShader::GetSamplerLocation(&m_SamplerLocation, m_pPixelShader, TEXTURE_SAMPLER_NAME[0]))
        return false;

    GX2InitSampler(&m_Sampler, GX2_TEX_CLAMP_MIRROR, GX2_TEX_XY_FILTER_BILINEAR);

    return true;
}

void FFLiCopySurfaceShader::SetupGPU()
{
    InvalidateFetchShader(&m_FetchShader.fetchShader);
    InvalidateVertexShader(m_pVertexShader);
    InvalidatePixelShader(m_pPixelShader);
}

void FFLiCopySurfaceShader::Bind() const
{
    GX2SetFetchShader(&m_FetchShader.fetchShader);
    GX2SetVertexShader(m_pVertexShader);
    GX2SetPixelShader(m_pPixelShader);
}

void FFLiCopySurfaceShader::SetTexture(const GX2Texture* pGX2Texture, u32 mipLevel)
{
    GX2InitSamplerLOD(&m_Sampler, mipLevel, mipLevel, 0.0f);

    GX2SetPixelSampler(&m_Sampler, m_SamplerLocation);
    GX2SetPixelTexture(pGX2Texture, m_SamplerLocation);
}

u32 FFLiCopySurfaceShader::GetBufferSize()
{
    return FFLiUtilShader::GetBufferSizeForFetch(2);
}
