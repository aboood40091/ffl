#ifndef FFLI_UTIL_SHADER_H_
#define FFLI_UTIL_SHADER_H_

#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <nn/ffl/types.h>

#include <gx2/shaders.h>

class FFLiUtilShader
{
public:
    struct FetchShader
    {
        GX2FetchShader      fetchShader;
        GX2AttribStream*    pStreams;
    };
    NN_STATIC_ASSERT(sizeof(FetchShader) == 0x24);

    struct AttributeStreamDesc
    {
        const char*     name;
        u32             buffer;
        u32             offset;
        GX2AttribFormat format;
    };
    NN_STATIC_ASSERT(sizeof(AttributeStreamDesc) == 0x10);

public:
    static bool BuildShader(FFLiUtilShader::FetchShader* pFetchShader, const GX2VertexShader* pVertexShader, const FFLiUtilShader::AttributeStreamDesc* pAttribs, u32 numAttribs, bool invalidate);

    static bool GetSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);
};

#endif // RIO_IS_CAFE

#endif // FFLI_UTIL_SHADER_H_
