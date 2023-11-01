#ifndef FFLI_COPY_SURFACE_DRAWER_H_
#define FFLI_COPY_SURFACE_DRAWER_H_

#include <nn/ffl/FFLVec.h>

#include <gpu/rio_VertexArray.h>

class FFLiBufferAllocator;

class FFLiCopySurfaceDrawer
{
public:
    FFLiCopySurfaceDrawer();
    ~FFLiCopySurfaceDrawer();

    void SetupCPU(FFLiBufferAllocator* pAllocator);
    void SetupGPU(u32 positionLocation, u32 texCoordLocation);

    void SetAttributeBuffer();
    void Draw();

    static u32 GetBufferSize();

private:
    struct Attribute
    {
        Attribute()
            : pBuffer(NULL)
        {
        }

        FFLVec2*            pBuffer;
        rio::VertexStream   vertexStream;
        rio::VertexBuffer   vertexBuffer;
    };

    Attribute           m_Position;
    Attribute           m_TexCoord;
    rio::VertexArray    m_VertexArray;
};
//NN_STATIC_ASSERT(sizeof(FFLiCopySurfaceDrawer) == 8);

#endif // FFLI_COPY_SURFACE_DRAWER_H_
