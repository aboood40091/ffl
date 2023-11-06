#include <nn/ffl/detail/FFLiCopySurfaceDrawer.h>

#include <gpu/rio_Drawer.h>
#include <misc/rio_MemUtil.h>

#include <cstring>

FFLiCopySurfaceDrawer::FFLiCopySurfaceDrawer()
{
    m_Position.pBuffer = nullptr;
    m_TexCoord.pBuffer = nullptr;
}

FFLiCopySurfaceDrawer::~FFLiCopySurfaceDrawer()
{
    if (m_Position.pBuffer != nullptr)
    {
        rio::MemUtil::free(m_Position.pBuffer);
        m_Position.pBuffer = nullptr;
    }
    if (m_TexCoord.pBuffer != nullptr)
    {
        rio::MemUtil::free(m_TexCoord.pBuffer);
        m_TexCoord.pBuffer = nullptr;
    }
}

void FFLiCopySurfaceDrawer::SetupCPU()
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

    m_Position.pBuffer = static_cast<FFLVec2*>(rio::MemUtil::alloc(POSITION_BUFFER_SIZE, rio::Drawer::cVtxAlignment));
    m_TexCoord.pBuffer = static_cast<FFLVec2*>(rio::MemUtil::alloc(TEXCOORD_BUFFER_SIZE, rio::Drawer::cVtxAlignment));

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
