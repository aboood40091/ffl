#ifndef FFLI_COMPRESSOR_IMPL_H_
#define FFLI_COMPRESSOR_IMPL_H_

#include <nn/ffl/FFLiCompressShader.h>
#include <nn/ffl/FFLiContextState.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiCompressDrawer.h>

#include <cafe/gx2.h>

template <typename T>
class FFLiCompressorImpl
{
public:
    FFLiCompressorImpl(FFLiBufferAllocator* pAllocator, const void* pShaderData, u32 shaderIndex);
    ~FFLiCompressorImpl();

private:
    FFLiBufferAllocator m_BufferAllocator;
    FFLiContextState    m_ContextState;
    FFLiCompressShader  m_CompressShader;
    FFLiCompressDrawer  m_CompressDrawer;
    GX2Texture          m_GX2Texture;
};

struct FFLiCompressorDescBC1;
struct FFLiCompressorDescBC3;

typedef FFLiCompressorImpl<FFLiCompressorDescBC1> FFLiCompressorImplBC1;
typedef FFLiCompressorImpl<FFLiCompressorDescBC3> FFLiCompressorImplBC3;

NN_STATIC_ASSERT(sizeof(FFLiCompressorImplBC1) == 0xF8);
NN_STATIC_ASSERT(sizeof(FFLiCompressorImplBC3) == 0xF8);

#endif // FFLI_COMPRESSOR_IMPL_H_
