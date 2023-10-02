#ifndef FFLI_UTIL_SHADER_H_
#define FFLI_UTIL_SHADER_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

class FFLiUtilShaderAllocator;

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
    static u32 GetBufferSize(u32 numAttribs, const void* pShaderData, u32 shaderIndex);

    static bool BuildShader(FFLiUtilShader::FetchShader* pFetchShader, const GX2VertexShader* pVertexShader, FFLiUtilShaderAllocator* pAllocator, const FFLiUtilShader::AttributeStreamDesc* pAttribs, u32 numAttribs, bool invalidate);
    static bool BuildShader(GX2PixelShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate);
    static bool BuildShader(GX2VertexShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate);

    static bool GetUniformLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);
    static bool GetUniformLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name);

    static bool GetUniformBlockLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);
    static bool GetUniformBlockLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name);

    static bool GetSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);
};

#endif // FFLI_UTIL_SHADER_H_
