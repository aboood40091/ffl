#include <nn/ffl/FFLiUtilShader.h>
#include <nn/ffl/FFLiUtilShaderAllocator.h>

FFLiUtilShaderAllocator::FFLiUtilShaderAllocator(FFLiBufferAllocator* pAllocator, u32 numAttribs, const void* pShaderData, u32 shaderIndex)
{
    u32 programBufferSize = FFLiUtilShader::GetProgramBufferSize(numAttribs, pShaderData, shaderIndex);
    void* pProgramBuffer = pAllocator->Allocate(programBufferSize, GX2_SHADER_ALIGNMENT);
    m_BufferAllocatorProgram.Init(pProgramBuffer, programBufferSize);

    u32 headerBufferSize = FFLiUtilShader::GetHeaderBufferSize(numAttribs, pShaderData, shaderIndex);
    void* pHeaderBuffer = pAllocator->Allocate(headerBufferSize, 4);
    m_BufferAllocatorHeader.Init(pHeaderBuffer, headerBufferSize);
}

FFLiUtilShaderAllocator::FFLiUtilShaderAllocator(FFLiBufferAllocator* pAllocator, u32 numAttribs)
{
    u32 programBufferSize = FFLiUtilShader::GetProgramBufferSizeForFetch(numAttribs);
    void* pProgramBuffer = pAllocator->Allocate(programBufferSize, GX2_SHADER_ALIGNMENT);
    m_BufferAllocatorProgram.Init(pProgramBuffer, programBufferSize);

    u32 headerBufferSize = FFLiUtilShader::GetHeaderBufferSizeForFetch(numAttribs);
    void* pHeaderBuffer = pAllocator->Allocate(headerBufferSize, 4);
    m_BufferAllocatorHeader.Init(pHeaderBuffer, headerBufferSize);
}
