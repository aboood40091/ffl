#ifndef FFLI_COMPRESSOR_PARAM_H_
#define FFLI_COMPRESSOR_PARAM_H_

#include <nn/ffl/types.h>

class FFLiBufferAllocator;
class FFLiCompressUniform;

class FFLiCompressorParam
{
public:
    FFLiCompressorParam();
    ~FFLiCompressorParam(); // Deleted in NSMBU

    static u32 GetBufferSize(u32 numMips);

    void SetTexture(const GX2Texture* pGX2Texture);

    FFLiCompressUniform* GetUniform(u32 index) const;

    static FFLiCompressorParam* Create(u32 uniformNum, bool compressUseUB, FFLiBufferAllocator* pAllocator);

private:
    u32                     m_UniformNum;
    FFLiCompressUniform**   m_ppUniform;
};
NN_STATIC_ASSERT(sizeof(FFLiCompressorParam) == 8);

#endif // FFLI_COMPRESSOR_PARAM_H_
