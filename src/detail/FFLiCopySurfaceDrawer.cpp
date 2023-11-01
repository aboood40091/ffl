#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiCopySurfaceDrawer.h>

#include <gpu/rio_Drawer.h>

#include <cstring>

FFLiCopySurfaceDrawer::FFLiCopySurfaceDrawer()
{
}

FFLiCopySurfaceDrawer::~FFLiCopySurfaceDrawer()
{
}

void FFLiCopySurfaceDrawer::SetupCPU(FFLiBufferAllocator* pAllocator)
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec2) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    static const FFLVec2 POSITION_BUFFER[4] = {
        { -1.0f,  1.0f },
        { -1.0f, -1.0f },
        {  1.0f,  1.0f },
        {  1.0f, -1.0f }
    };
    NN_STATIC_ASSERT(sizeof(POSITION_BUFFER) == POSITION_BUFFER_SIZE);

    static const FFLVec2 TEXCOORD_BUFFER[4] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f }
    };
    NN_STATIC_ASSERT(sizeof(TEXCOORD_BUFFER) == TEXCOORD_BUFFER_SIZE);

    m_Position.pBuffer = static_cast<FFLVec2*>(pAllocator->Allocate(POSITION_BUFFER_SIZE, rio::Drawer::cVtxAlignment));
    m_TexCoord.pBuffer = static_cast<FFLVec2*>(pAllocator->Allocate(TEXCOORD_BUFFER_SIZE, rio::Drawer::cVtxAlignment));

    std::memcpy(m_Position.pBuffer, POSITION_BUFFER, POSITION_BUFFER_SIZE);
    std::memcpy(m_TexCoord.pBuffer, TEXCOORD_BUFFER, TEXCOORD_BUFFER_SIZE);
}

void FFLiCopySurfaceDrawer::SetupGPU(u32 positionLocation, u32 texCoordLocation)
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec2) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    m_Position.vertexStream.setLayout(positionLocation, rio::VertexStream::FORMAT_32_32_FLOAT, 0);
    m_Position.vertexBuffer.setStride(sizeof(FFLVec2));
    m_Position.vertexBuffer.setDataInvalidate(m_Position.pBuffer, POSITION_BUFFER_SIZE);

    m_TexCoord.vertexStream.setLayout(texCoordLocation, rio::VertexStream::FORMAT_32_32_FLOAT, 0);
    m_TexCoord.vertexBuffer.setStride(sizeof(FFLVec2));
    m_TexCoord.vertexBuffer.setDataInvalidate(m_TexCoord.pBuffer, TEXCOORD_BUFFER_SIZE);

    m_VertexArray.addAttribute(m_Position.vertexStream, m_Position.vertexBuffer);
    m_VertexArray.addAttribute(m_TexCoord.vertexStream, m_TexCoord.vertexBuffer);
    m_VertexArray.process();
}

void FFLiCopySurfaceDrawer::SetAttributeBuffer()
{
    m_VertexArray.bind();
}

void FFLiCopySurfaceDrawer::Draw()
{
    rio::Drawer::DrawArrays(rio::Drawer::TRIANGLE_STRIP, 4);
}

u32 FFLiCopySurfaceDrawer::GetBufferSize()
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec2) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    u32 ret  = rio::Drawer::cVtxAlignment;
    ret     += POSITION_BUFFER_SIZE;

    ret     += rio::Drawer::cVtxAlignment;
    ret     += TEXCOORD_BUFFER_SIZE;

    return ret;
}
