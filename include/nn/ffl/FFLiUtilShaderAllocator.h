#ifndef FFLI_UTIL_SHADER_ALLOCATOR_H_
#define FFLI_UTIL_SHADER_ALLOCATOR_H_

#include <nn/ffl/detail/FFLiBufferAllocator.h>

class FFLiUtilShaderAllocator
{
public:
    FFLiUtilShaderAllocator(FFLiBufferAllocator* pAllocator, u32 numAttribs, const void* pShaderData, u32 shaderIndex);

    FFLiBufferAllocator& HeaderAllocator()
    {
        return m_BufferAllocatorHeader;
    }

    FFLiBufferAllocator& ProgramAllocator()
    {
        return m_BufferAllocatorProgram;
    }

private:
    FFLiBufferAllocator m_BufferAllocatorHeader;
    FFLiBufferAllocator m_BufferAllocatorProgram;
};
NN_STATIC_ASSERT(sizeof(FFLiUtilShaderAllocator) == 0x10);

#endif // FFLI_UTIL_SHADER_ALLOCATOR_H_
