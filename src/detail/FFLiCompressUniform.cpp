#include <nn/ffl/FFLiSwapEndian.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiCompressUniform.h>

#include <cafe/gx2.h>

namespace {

u32 GetBlockAlignedSize(u32 size);

template <typename T>
void SetupUniformBlock(T* pArray, u32 arraySize, u32 size);

}

FFLiCompressUniform::FFLiCompressUniform()
    : m_IsUB(false)
    , m_pVertexUniform(NULL)
    , m_pPixelUniform(NULL)
{
}

void FFLiCompressUniform::Setup(FFLiBufferAllocator* pAllocator, u32 compressUseUB)
{
    const u32 VERTEX_UNIFORM_MAX_SIZE = (sizeof(f32) * 4) + (GX2_UNIFORM_BLOCK_ALIGNMENT - 1) & ~(GX2_UNIFORM_BLOCK_ALIGNMENT - 1);
    const u32 PIXEL_UNIFORM_MAX_SIZE = (sizeof(f32) * 4) + (GX2_UNIFORM_BLOCK_ALIGNMENT - 1) & ~(GX2_UNIFORM_BLOCK_ALIGNMENT - 1);

    m_IsUB = compressUseUB;

    m_pVertexUniform = static_cast<f32*>(pAllocator->Allocate(VERTEX_UNIFORM_MAX_SIZE, GX2_UNIFORM_BLOCK_ALIGNMENT));
    m_pPixelUniform  = static_cast<f32*>(pAllocator->Allocate(PIXEL_UNIFORM_MAX_SIZE, GX2_UNIFORM_BLOCK_ALIGNMENT));
}

void FFLiCompressUniform::SetResolution(u32 width, u32 height)
{
    const u32 VERTEX_UNIFORM_MAX_SIZE = (sizeof(f32) * 4) + (GX2_UNIFORM_BLOCK_ALIGNMENT - 1) & ~(GX2_UNIFORM_BLOCK_ALIGNMENT - 1);

    f32 widthf  = width;
    f32 heightf = height;

    f32 alignedWidth  = GetBlockAlignedSize(width)  / widthf;
    f32 alignedHeight = GetBlockAlignedSize(height) / heightf;

    m_pVertexUniform[0] = 1 / widthf;
    m_pVertexUniform[1] = 1 / heightf;
    m_pVertexUniform[2] = alignedWidth;
    m_pVertexUniform[3] = alignedHeight;

    if (m_IsUB)
        SetupUniformBlock(m_pVertexUniform, 4, VERTEX_UNIFORM_MAX_SIZE);
}

void FFLiCompressUniform::SetMipLevel(u32 mipLevel)
{
    const u32 PIXEL_UNIFORM_MAX_SIZE = (sizeof(f32) * 4) + (GX2_UNIFORM_BLOCK_ALIGNMENT - 1) & ~(GX2_UNIFORM_BLOCK_ALIGNMENT - 1);

    m_pPixelUniform[0] = mipLevel;
    m_pPixelUniform[1] = 0;
    m_pPixelUniform[2] = 0;
    m_pPixelUniform[3] = 0;

    if (m_IsUB)
        SetupUniformBlock(m_pPixelUniform, 4, PIXEL_UNIFORM_MAX_SIZE);
}

u32 FFLiCompressUniform::GetBufferSize()
{
    const u32 VERTEX_UNIFORM_MAX_SIZE = (sizeof(f32) * 4) + (GX2_UNIFORM_BLOCK_ALIGNMENT - 1) & ~(GX2_UNIFORM_BLOCK_ALIGNMENT - 1);
    const u32 PIXEL_UNIFORM_MAX_SIZE = (sizeof(f32) * 4) + (GX2_UNIFORM_BLOCK_ALIGNMENT - 1) & ~(GX2_UNIFORM_BLOCK_ALIGNMENT - 1);

    u32 ret  = GX2_UNIFORM_BLOCK_ALIGNMENT;
    ret     += VERTEX_UNIFORM_MAX_SIZE;
    ret     += PIXEL_UNIFORM_MAX_SIZE;

    return ret;
}

namespace {

u32 GetBlockAlignedSize(u32 size)
{
    return size + (4 - 1) & ~(4 - 1);
}

template <typename T>
void SetupUniformBlock(T* pArray, u32 arraySize, u32 size)
{
    FFLiSwapEndianArrayImpl(pArray, arraySize);
    GX2Invalidate(GX2_INVALIDATE_CPU_UNIFORM_BLOCK, pArray, size);
}

}
