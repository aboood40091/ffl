#ifndef FFLI_COMPRESSOR_H_
#define FFLI_COMPRESSOR_H_

#include <nn/ffl/detail/FFLiCompressorImpl.h>

class FFLiCompressor
{
public:
    FFLiCompressor(FFLiBufferAllocator* pAllocator, const void* pShaderData);
    ~FFLiCompressor();

    FFLiCompressorImplBC1& GetCompressorImplBC1()
    {
        return m_CompressorImplBC1;
    }

    FFLiCompressorImplBC3& GetCompressorImplBC3()
    {
        return m_CompressorImplBC3;
    }

    bool UseUB() const;

private:
    FFLiCompressorImplBC1   m_CompressorImplBC1;
    FFLiCompressorImplBC3   m_CompressorImplBC3;
};
NN_STATIC_ASSERT(sizeof(FFLiCompressor) == 0x1F0);

#endif // FFLI_COMPRESSOR_H_
