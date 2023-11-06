#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiUtil.h>
#include <nn/ffl/FFLiUtilShader.h>
#include <nn/ffl/FFLiUtilShaderAllocator.h>

#include <nn/ffl/detail/FFLiBug.h>

#include <cafe/gfd.h>

static void InvalidateFetchShader(GX2FetchShader* pShader);

static bool BuildPixelShader(GX2PixelShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate);
static bool BuildVertexShader(GX2VertexShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate);

static bool GetPixelUniformLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);
static bool GetVertexUniformLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name);

static bool GetPixelUniformBlockLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);
static bool GetVertexUniformBlockLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name);

static bool GetPixelSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name);

u32 FFLiUtilShader::GetBufferSize(u32 numAttribs, const void* pShaderData, u32 shaderIndex)
{
    u32 ret  = GX2_SHADER_ALIGNMENT;
    ret     += GetProgramBufferSize(numAttribs, pShaderData, shaderIndex);
    ret     += GetHeaderBufferSize(numAttribs, pShaderData, shaderIndex);

    return ret;
}

u32 FFLiUtilShader::GetBufferSizeForFetch(u32 numAttribs)
{
    u32 ret  = GX2_SHADER_ALIGNMENT;
    ret     += GetProgramBufferSizeForFetch(numAttribs);
    ret     += GetHeaderBufferSizeForFetch(numAttribs);

    return ret;
}

u32 FFLiUtilShader::GetHeaderBufferSize(u32 numAttribs, const void* pShaderData, u32 shaderIndex)
{
    u32 ret  = GetHeaderBufferSizeForFetch(numAttribs);
    ret     += FFLiRoundUp(GFDGetPixelShaderHeaderSize(shaderIndex, pShaderData), 4);
    ret     += FFLiRoundUp(GFDGetVertexShaderHeaderSize(shaderIndex, pShaderData), 4);

    return ret;
}

u32 FFLiUtilShader::GetHeaderBufferSizeForFetch(u32 numAttribs)
{
    return FFLiRoundUp(sizeof(GX2AttribStream) * numAttribs, 4);
}

u32 FFLiUtilShader::GetProgramBufferSize(u32 numAttribs, const void* pShaderData, u32 shaderIndex)
{
    u32 ret  = GetProgramBufferSizeForFetch(numAttribs);
    ret     += FFLiRoundUp(GFDGetPixelShaderProgramSize(shaderIndex, pShaderData), GX2_SHADER_ALIGNMENT);
    ret     += FFLiRoundUp(GFDGetVertexShaderProgramSize(shaderIndex, pShaderData), GX2_SHADER_ALIGNMENT);

    return ret;
}

u32 FFLiUtilShader::GetProgramBufferSizeForFetch(u32 numAttribs)
{
    return FFLiRoundUp(GX2CalcFetchShaderSize(numAttribs), GX2_SHADER_ALIGNMENT);
}

bool FFLiUtilShader::BuildShader(FFLiUtilShader::FetchShader* pFetchShader, const GX2VertexShader* pVertexShader, FFLiUtilShaderAllocator* pAllocator, const FFLiUtilShader::AttributeStreamDesc* pAttribs, u32 numAttribs, bool invalidate)
{
    u32 size = GX2CalcFetchShaderSize(numAttribs);
    void* pBuffer = pAllocator->ProgramAllocator().Allocate(size, GX2_SHADER_ALIGNMENT);

    pFetchShader->pStreams = static_cast<GX2AttribStream*>(pAllocator->HeaderAllocator().Allocate(sizeof(GX2AttribStream) * numAttribs, 4));

    for (u32 i = 0; i < numAttribs; i++)
    {
        s32 location = GX2GetVertexAttribVarLocation(pVertexShader, pAttribs[i].name);
        if (location < 0)
            return false;

        GX2InitAttribStream(&(pFetchShader->pStreams[i]), location, pAttribs[i].buffer, pAttribs[i].offset, pAttribs[i].format);
    }

    GX2InitFetchShader(&pFetchShader->fetchShader, pBuffer, numAttribs, pFetchShader->pStreams);
    FFLiBugEndianSwap(pBuffer, size);

    if (invalidate)
        InvalidateFetchShader(&pFetchShader->fetchShader);

    return true;
}

bool FFLiUtilShader::BuildShader(GX2PixelShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate)
{
    return BuildPixelShader(ppShader, pAllocator, pShaderData, shaderIndex, invalidate);
}

bool FFLiUtilShader::BuildShader(GX2VertexShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate)
{
    return BuildVertexShader(ppShader, pAllocator, pShaderData, shaderIndex, invalidate);
}

bool FFLiUtilShader::GetUniformLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    return GetPixelUniformLocation(pLocation, pShader, name);
}

bool FFLiUtilShader::GetUniformLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name)
{
    return GetVertexUniformLocation(pLocation, pShader, name);
}

bool FFLiUtilShader::GetUniformBlockLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    return GetPixelUniformBlockLocation(pLocation, pShader, name);
}

bool FFLiUtilShader::GetUniformBlockLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name)
{
    return GetVertexUniformBlockLocation(pLocation, pShader, name);
}

bool FFLiUtilShader::GetSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    return GetPixelSamplerLocation(pLocation, pShader, name);
}

static void InvalidateFetchShader(GX2FetchShader* pShader)
{
    FFLiInvalidateShader(pShader->shaderPtr, pShader->shaderSize);
}

static bool BuildPixelShader(GX2PixelShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate)
{
    if (shaderIndex >= GFDGetPixelShaderCount(pShaderData))
        return false;

    u32 headerSize = GFDGetPixelShaderHeaderSize(shaderIndex, pShaderData);
    u32 programSize = GFDGetPixelShaderProgramSize(shaderIndex, pShaderData);

    if (headerSize == 0 || programSize == 0)
        return false;

    GX2PixelShader* pHeader = static_cast<GX2PixelShader*>(pAllocator->HeaderAllocator().Allocate(headerSize, 4));
    void* pProgram = pAllocator->ProgramAllocator().Allocate(programSize, GX2_SHADER_ALIGNMENT);

    if (!GFDGetPixelShader(pHeader, pProgram, shaderIndex, pShaderData))
        return false;

    FFLiBugEndianSwap(pProgram, programSize);

    *ppShader = pHeader;

    if (invalidate)
        FFLiInvalidateShader(pHeader->shaderPtr, pHeader->shaderSize);

    return true;
}

static bool BuildVertexShader(GX2VertexShader** ppShader, FFLiUtilShaderAllocator* pAllocator, const void* pShaderData, u32 shaderIndex, bool invalidate)
{
    if (shaderIndex >= GFDGetVertexShaderCount(pShaderData))
        return false;

    u32 headerSize = GFDGetVertexShaderHeaderSize(shaderIndex, pShaderData);
    u32 programSize = GFDGetVertexShaderProgramSize(shaderIndex, pShaderData);

    if (headerSize == 0 || programSize == 0)
        return false;

    GX2VertexShader* pHeader = static_cast<GX2VertexShader*>(pAllocator->HeaderAllocator().Allocate(headerSize, 4));
    void* pProgram = pAllocator->ProgramAllocator().Allocate(programSize, GX2_SHADER_ALIGNMENT);

    if (!GFDGetVertexShader(pHeader, pProgram, shaderIndex, pShaderData))
        return false;

    FFLiBugEndianSwap(pProgram, programSize);

    *ppShader = pHeader;

    if (invalidate)
        FFLiInvalidateShader(pHeader->shaderPtr, pHeader->shaderSize);

    return true;
}

static bool GetPixelUniformLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    s32 location = GX2GetPixelUniformVarOffset(pShader, name);
    if (location < 0)
        return false;

    *pLocation = location;
    return true;
}

static bool GetVertexUniformLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name)
{
    s32 location = GX2GetVertexUniformVarOffset(pShader, name);
    if (location < 0)
        return false;

    *pLocation = location;
    return true;
}

static bool GetPixelUniformBlockLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    const GX2UniformBlock* pUB = GX2GetPixelUniformBlock(pShader, name);
    if (pUB == NULL)
        return false;

    *pLocation = pUB->offset;
    return true;
}

static bool GetVertexUniformBlockLocation(u32* pLocation, const GX2VertexShader* pShader, const char* name)
{
    const GX2UniformBlock* pUB = GX2GetVertexUniformBlock(pShader, name);
    if (pUB == NULL)
        return false;

    *pLocation = pUB->offset;
    return true;
}

static bool GetPixelSamplerLocation(u32* pLocation, const GX2PixelShader* pShader, const char* name)
{
    s32 location = GX2GetPixelSamplerVarLocation(pShader, name);
    if (location < 0)
        return false;

    *pLocation = location;
    return true;
}
