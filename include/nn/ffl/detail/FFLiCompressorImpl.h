#ifndef FFLI_COMPRESSOR_IMPL_H_
#define FFLI_COMPRESSOR_IMPL_H_

#include <nn/ffl/FFLiCompressShader.h>
#include <nn/ffl/FFLiContextState.h>
#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiTemp.h>
#include <nn/ffl/FFLiTexture.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiCompressDrawer.h>
#include <nn/ffl/detail/FFLiCompressorDesc.h>
#include <nn/ffl/detail/FFLiCompressorParam.h>

#include <cafe/gx2.h>

template <typename T>
class FFLiCompressorImpl
{
public:
    FFLiCompressorImpl(FFLiBufferAllocator* pAllocator, const void* pShaderData, u32 shaderIndex);
    ~FFLiCompressorImpl();

    static u32 GetBufferSize(const void* pShaderData, u32 shaderIndex);
    static u32 GetTextureBufferSize(u32 width, u32 height, u32 numMips);

    bool SetupCPU(const void* pShaderData, u32 shaderIndex);
    void SetupGPU();

    bool UseUB() const;

    void CompressImpl(GX2Texture* pDst, const GX2Texture* pSrc, const FFLiCompressorParam* pParam);

private:
    FFLiBufferAllocator m_BufferAllocator;
    FFLiContextState    m_ContextState;
    FFLiCompressShader  m_CompressShader;
    FFLiCompressDrawer  m_CompressDrawer;
    GX2ColorBuffer      m_ColorBuffer;
};

typedef FFLiCompressorImpl<FFLiCompressorDescBC1> FFLiCompressorImplBC1;
typedef FFLiCompressorImpl<FFLiCompressorDescBC3> FFLiCompressorImplBC3;

NN_STATIC_ASSERT(sizeof(FFLiCompressorImplBC1) == 0xF8);
NN_STATIC_ASSERT(sizeof(FFLiCompressorImplBC3) == 0xF8);

template <typename T>
FFLiCompressorImpl<T>::FFLiCompressorImpl(FFLiBufferAllocator* pAllocator, const void* pShaderData, u32 shaderIndex)
    : m_BufferAllocator()
    , m_ContextState()
    , m_CompressShader()
    , m_CompressDrawer()
    , m_ColorBuffer()
{
    u32 bufferSize = GetBufferSize(pShaderData, shaderIndex);
    void* pBuffer = pAllocator->Allocate(bufferSize);

    m_BufferAllocator.Init(pBuffer, bufferSize);
}

template <typename T>
FFLiCompressorImpl<T>::~FFLiCompressorImpl()
{
}

template <typename T>
u32 FFLiCompressorImpl<T>::GetBufferSize(const void* pShaderData, u32 shaderIndex)
{
    u32 bufferSize = FFLiContextState::GetBufferSize();
    bufferSize    += FFLiCompressShader::GetBufferSize(pShaderData, shaderIndex);
    bufferSize    += FFLiCompressDrawer::GetBufferSize();

    return bufferSize;
}

template <typename T>
u32 FFLiCompressorImpl<T>::GetTextureBufferSize(u32 width, u32 height, u32 numMips)
{
    GX2Texture texture;
    GX2InitTexture(&texture, width, height, 1, numMips, T::FORMAT, GX2_SURFACE_DIM_2D);
    return texture.surface.alignment * 2 + texture.surface.imageSize + texture.surface.mipSize;
}

template <typename T>
bool FFLiCompressorImpl<T>::SetupCPU(const void* pShaderData, u32 shaderIndex)
{
    m_ContextState.Initialize(&m_BufferAllocator);
    if (m_CompressShader.SetupCPU(pShaderData, shaderIndex, &m_BufferAllocator))
    {
        m_CompressDrawer.SetupCPU(&m_BufferAllocator);
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
void FFLiCompressorImpl<T>::SetupGPU()
{
    m_CompressShader.SetupGPU();
    m_CompressDrawer.SetupGPU();
    m_ContextState.Setup();
    m_ContextState.Bind();
    FFLiTempSetShaderMode(UseUB() ? GX2_SHADER_MODE_UNIFORM_BLOCK : GX2_SHADER_MODE_UNIFORM_REGISTER);
    GX2SetTargetChannelMasks(
        GX2_CHANNEL_MASK_RGBA,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE
    );
    GX2SetAlphaTest(GX2_DISABLE, GX2_COMPARE_ALWAYS, 0.0f);
    GX2SetColorControl(GX2_LOGIC_OP_NONE, 0xFF, GX2_DISABLE, GX2_ENABLE);
    GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_LESS);
    GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, GX2_DISABLE, GX2_ENABLE);
    GX2SetBlendControl(
        GX2_RENDER_TARGET_0,
        
        GX2_BLEND_ONE,
        GX2_BLEND_ZERO,
        GX2_BLEND_COMBINE_ADD,
        
        GX2_DISABLE,
        
        GX2_BLEND_ONE,
        GX2_BLEND_ZERO,
        GX2_BLEND_COMBINE_ADD
    );
    m_CompressShader.Bind();
    FFLiTempSetContextState(NULL);
}

template <typename T>
bool FFLiCompressorImpl<T>::UseUB() const
{
    return m_CompressShader.UseUB();
}

template <typename T>
void FFLiCompressorImpl<T>::CompressImpl(GX2Texture* pDst, const GX2Texture* pSrc, const FFLiCompressorParam* pParam)
{
    m_ContextState.Bind();

    GX2ColorBuffer* pColorBuffer = &m_ColorBuffer;

    m_CompressShader.SetTexture(pSrc);
    m_CompressDrawer.SetAttributeBuffer(0, 1);

    for (u32 i = 0; i < pDst->surface.numMips; i++)
    {
        T::SetupColorBuffer(pColorBuffer, pDst, i);
        
        GX2SetColorBuffer(pColorBuffer, GX2_RENDER_TARGET_0);
        
        u32 width = FFLiCalculateUnitTextureResolution(pDst->surface.width, i) >> (i & 0x3f);
        u32 height = FFLiCalculateUnitTextureResolution(pDst->surface.height, i) >> (i & 0x3f);
        GX2SetViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f);
        GX2SetScissor(0, 0, width, height);

        m_CompressShader.SetUniform(pParam->GetUniform(i));
        m_CompressDrawer.Draw();
    }

    FFLiInvalidateColorBuffer(pColorBuffer);
    FFLiInvalidateTexture(pDst);
}

#endif // FFLI_COMPRESSOR_IMPL_H_
