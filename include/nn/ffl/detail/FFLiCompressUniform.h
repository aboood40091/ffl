#ifndef FFLI_COMPRESS_UNIFORM_H_
#define FFLI_COMPRESS_UNIFORM_H_

#include <nn/ffl/types.h>

class FFLiCompressUniform
{
public:
    FFLiCompressUniform();
    ~FFLiCompressUniform(); // Deleted in NSMBU

    const void* GetVertexUniform() const
    {
        return m_pVertexUniform;
    }

    const void* GetPixelUniform() const
    {
        return m_pPixelUniform;
    }

private:
    bool        _0;
    const void* m_pVertexUniform;
    const void* m_pPixelUniform;
};
NN_STATIC_ASSERT(sizeof(FFLiCompressUniform) == 0xC);

#endif // FFLI_COMPRESS_UNIFORM_H_
