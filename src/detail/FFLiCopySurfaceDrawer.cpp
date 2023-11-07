#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <nn/ffl/detail/FFLiCopySurfaceDrawer.h>

#include <misc/rio_MemUtil.h>

#include <gx2/draw.h>
#include <gx2/mem.h>

#include <cstring>

FFLiCopySurfaceDrawer::FFLiCopySurfaceDrawer()
    : m_pPositionBuffer(NULL)
    , m_pTexCoordBuffer(NULL)
{
}

FFLiCopySurfaceDrawer::~FFLiCopySurfaceDrawer()
{
    if (m_pPositionBuffer != NULL)
    {
        rio::MemUtil::free(m_pPositionBuffer);
        m_pPositionBuffer = NULL;
    }
    if (m_pTexCoordBuffer != NULL)
    {
        rio::MemUtil::free(m_pTexCoordBuffer);
        m_pTexCoordBuffer = NULL;
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

    m_pPositionBuffer = static_cast<FFLVec2*>(rio::MemUtil::alloc(POSITION_BUFFER_SIZE, GX2_VERTEX_BUFFER_ALIGNMENT));
    m_pTexCoordBuffer = static_cast<FFLVec2*>(rio::MemUtil::alloc(TEXCOORD_BUFFER_SIZE, GX2_VERTEX_BUFFER_ALIGNMENT));

    std::memcpy(m_pPositionBuffer, POSITION_BUFFER, POSITION_BUFFER_SIZE);
    std::memcpy(m_pTexCoordBuffer, TEXCOORD_BUFFER, TEXCOORD_BUFFER_SIZE);
}

void FFLiCopySurfaceDrawer::SetupGPU()
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec2) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, m_pPositionBuffer, POSITION_BUFFER_SIZE);
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, m_pTexCoordBuffer, TEXCOORD_BUFFER_SIZE);
}

void FFLiCopySurfaceDrawer::SetAttributeBuffer(u32 positionBufferIndex, u32 texCoordBufferIndex)
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec2) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    GX2SetAttribBuffer(positionBufferIndex, POSITION_BUFFER_SIZE, sizeof(FFLVec2), m_pPositionBuffer);
    GX2SetAttribBuffer(texCoordBufferIndex, TEXCOORD_BUFFER_SIZE, sizeof(FFLVec2), m_pTexCoordBuffer);
}

void FFLiCopySurfaceDrawer::Draw()
{
    GX2DrawEx(GX2_PRIMITIVE_MODE_TRIANGLE_STRIP, 4, 0, 1);
}

#endif // RIO_IS_CAFE
