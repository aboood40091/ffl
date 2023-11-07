#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <nn/ffl/FFLiUtilShader.h>

#include <nn/ffl/detail/FFLiBug.h>

#include <misc/rio_MemUtil.h>

#include <gx2/mem.h>
#define GX2_SHADER_ALIGNMENT 0x100

namespace {

static inline GX2AttribVar* GX2GetVertexAttribVar(const GX2VertexShader* shader, const char* name)
{
    for (uint32_t i = 0; i < shader->attribVarCount; i++)
    {
       if (strcmp(shader->attribVars[i].name, name) == 0)
           return &(shader->attribVars[i]);
    }

    return nullptr;
}

static inline s32 GX2GetVertexAttribVarLocation(const GX2VertexShader* shader, const char* name)
{
    GX2AttribVar* attrib = GX2GetVertexAttribVar(shader, name);
    if (!attrib)
        return -1;

    return attrib->location;
}

static inline GX2SamplerVar* GX2GetPixelSamplerVar(const GX2PixelShader* shader, const char* name)
{
    for (uint32_t i = 0; i < shader->samplerVarCount; i++)
    {
       if (strcmp(shader->samplerVars[i].name, name) == 0)
           return &(shader->samplerVars[i]);
    }

    return nullptr;
}

static inline s32 GX2GetPixelSamplerVarLocation(const GX2PixelShader* shader, const char* name)
{
    GX2SamplerVar* sampler = GX2GetPixelSamplerVar(shader, name);
    if (!sampler)
        return -1;

    return sampler->location;
}

}

static void InvalidateFetchShader(GX2FetchShader* pShader);

static bool GetPixelSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);

bool FFLiUtilShader::BuildShader(FFLiUtilShader::FetchShader* pFetchShader, const GX2VertexShader* pVertexShader, const FFLiUtilShader::AttributeStreamDesc* pAttribs, u32 numAttribs, bool invalidate)
{
    static const u32 compMap[20] = {
        0x00040405, 0x00010405,
        0x00040405, 0x00040405,
        0x00010405, 0x00040405,
        0x00040405, 0x00010405,
        0x00010405, 0x00010205,
        0x00010203, 0x00010203,
        0x00010405, 0x00010405,
        0x00010203, 0x00010203,
        0x00010205, 0x00010205,
        0x00010203, 0x00010203
    };

    u32 size = GX2CalcFetchShaderSizeEx(numAttribs, GX2_FETCH_SHADER_TESSELLATION_NONE, GX2_TESSELLATION_MODE_DISCRETE);
    void* pBuffer = rio::MemUtil::alloc(size, GX2_SHADER_ALIGNMENT);

    pFetchShader->pStreams = new GX2AttribStream[numAttribs];

    for (u32 i = 0; i < numAttribs; i++)
    {
        s32 location = GX2GetVertexAttribVarLocation(pVertexShader, pAttribs[i].name);
        if (location < 0)
            return false;

        GX2AttribStream& attribStream = pFetchShader->pStreams[i];
        GX2AttribFormat format = pAttribs[i].format;

        attribStream.buffer = pAttribs[i].buffer;
        attribStream.offset = pAttribs[i].offset;
        attribStream.location = location;
        attribStream.format = format;
        attribStream.mask = compMap[format & 0xff];
        attribStream.type = GX2_ATTRIB_INDEX_PER_VERTEX;
        attribStream.aluDivisor = 0;
        attribStream.endianSwap = GX2_ENDIAN_SWAP_DEFAULT;
    }

    GX2InitFetchShaderEx(&pFetchShader->fetchShader, (u8*)pBuffer, numAttribs, pFetchShader->pStreams, GX2_FETCH_SHADER_TESSELLATION_NONE, GX2_TESSELLATION_MODE_DISCRETE);
    FFLiBugEndianSwap(pBuffer, size);

    if (invalidate)
        InvalidateFetchShader(&pFetchShader->fetchShader);

    return true;
}

void FFLiUtilShader::DestroyShader(FFLiUtilShader::FetchShader* pFetchShader)
{
    if (pFetchShader->fetchShader.program != nullptr)
    {
        rio::MemUtil::free(pFetchShader->fetchShader.program);
        pFetchShader->fetchShader.program = nullptr;
    }
    if (pFetchShader->pStreams != nullptr)
    {
        delete[] pFetchShader->pStreams;
        pFetchShader->pStreams = nullptr;
    }
}

bool FFLiUtilShader::GetSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    return GetPixelSamplerLocation(pLocation, pShader, name);
}

static void FFLiInvalidateShader(void* ptr, u32 size)
{
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, ptr, size);
}

static void InvalidateFetchShader(GX2FetchShader* pShader)
{
    FFLiInvalidateShader(pShader->program, pShader->size);
}

static bool GetPixelSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    s32 location = GX2GetPixelSamplerVarLocation(pShader, name);
    if (location < 0)
        return false;

    *pLocation = location;
    return true;
}

#endif // RIO_IS_CAFE
