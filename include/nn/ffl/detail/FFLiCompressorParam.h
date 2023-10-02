#ifndef FFLI_COMPRESSOR_PARAM_H_
#define FFLI_COMPRESSOR_PARAM_H_

#include <nn/ffl/types.h>

class FFLiCompressUniform;

class FFLiCompressorParam
{
public:
    FFLiCompressorParam();
    ~FFLiCompressorParam(); // Deleted in NSMBU

    FFLiCompressUniform* GetUniform(u32 index) const;

private:
    u32                     m_UniformNum;
    FFLiCompressUniform**   m_ppUniform;
};
NN_STATIC_ASSERT(sizeof(FFLiCompressorParam) == 8);

#endif // FFLI_COMPRESSOR_PARAM_H_
