#include <nn/ffl/FFLiMipMapUtil.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiCompressorParam.h>
#include <nn/ffl/detail/FFLiCompressUniform.h>

#include <new>

FFLiCompressorParam::FFLiCompressorParam()
    : m_NumMips(0)
    , m_ppUniform(NULL)
{
}

u32 FFLiCompressorParam::GetBufferSize(u32 numMips)
{
    u32 ret  = sizeof(FFLiCompressUniform*)         * numMips;
    ret     += sizeof(FFLiCompressUniform)          * numMips;
    ret     += FFLiCompressUniform::GetBufferSize() * numMips;

    return ret;
}

void FFLiCompressorParam::Setup(FFLiBufferAllocator* pAllocator, u32 numMips, bool compressUseUB)
{
    m_NumMips = numMips;
    m_ppUniform = static_cast<FFLiCompressUniform**>(pAllocator->Allocate(sizeof(FFLiCompressUniform*) * numMips, 4));
    for (u32 i = 0; i < numMips; i++)
        m_ppUniform[i] = new (pAllocator->Allocate(sizeof(FFLiCompressUniform), 4)) FFLiCompressUniform();

    for (u32 i = 0; i < numMips; i++)
        m_ppUniform[i]->Setup(pAllocator, compressUseUB);
}

void FFLiCompressorParam::SetTexture(const GX2Texture* pGX2Texture)
{
    for (u32 i = 0; i < pGX2Texture->surface.numMips; i++)
    {
        u32 width  = FFLiGetMipMapLevelSize(pGX2Texture->surface.width,  i);
        u32 height = FFLiGetMipMapLevelSize(pGX2Texture->surface.height, i);
        m_ppUniform[i]->SetResolution(width, height);
        m_ppUniform[i]->SetMipLevel(i);
    }
}

FFLiCompressUniform* FFLiCompressorParam::GetUniform(u32 mipLevel) const
{
    return m_ppUniform[mipLevel];
}

FFLiCompressorParam* FFLiCompressorParam::Create(u32 numMips, bool compressUseUB, FFLiBufferAllocator* pAllocator)
{
    FFLiCompressorParam* pParam = new (pAllocator->Allocate(sizeof(FFLiCompressorParam), 4)) FFLiCompressorParam();
    pParam->Setup(pAllocator, numMips, compressUseUB);
    return pParam;
}
