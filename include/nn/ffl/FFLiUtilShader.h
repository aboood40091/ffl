#ifndef FFLI_UTIL_SHADER_H_
#define FFLI_UTIL_SHADER_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

class FFLiUtilShader
{
public:
    struct FetchShader
    {
        GX2FetchShader      fetchShader;
        GX2AttribStream*    pStreams;
    };
    NN_STATIC_ASSERT(sizeof(FetchShader) == 0x24);
};

#endif // FFLI_UTIL_SHADER_H_
