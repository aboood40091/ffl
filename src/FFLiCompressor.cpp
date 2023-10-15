#include <nn/ffl/FFLiCompressor.h>

FFLiCompressor::FFLiCompressor(FFLiBufferAllocator* pAllocator, const void* pShaderData)
    : m_CompressorImplBC1(pAllocator, pShaderData, 0)
    , m_CompressorImplBC3(pAllocator, pShaderData, 1)
{
}

u32 FFLiCompressor::GetBufferSize(const void* pShaderData)
{
    u32 bufferSizeBC1 = FFLiCompressorImplBC1::GetBufferSize(pShaderData, 0);
    u32 bufferSizeBC3 = FFLiCompressorImplBC3::GetBufferSize(pShaderData, 1);

    if (bufferSizeBC1 == 0 || bufferSizeBC3 == 0)
        return 0;

    return bufferSizeBC1 + bufferSizeBC3;
}

bool FFLiCompressor::SetupCPU(const void* pShaderData)
{
    if (!m_CompressorImplBC1.SetupCPU(pShaderData, 0))
        return false;

    if (!m_CompressorImplBC3.SetupCPU(pShaderData, 1))
        return false;

    return true;
}

void FFLiCompressor::SetupGPU()
{
    m_CompressorImplBC1.SetupGPU();
    m_CompressorImplBC3.SetupGPU();
}

bool FFLiCompressor::UseUB() const
{
    return m_CompressorImplBC1.UseUB();
}
