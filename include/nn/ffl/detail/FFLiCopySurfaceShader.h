#ifndef FFLI_COPY_SURFACE_SHADER_H_
#define FFLI_COPY_SURFACE_SHADER_H_

#include <nn/ffl/types.h>

#include <common/aglTextureSampler.h>
#include <gpu/rio_Shader.h>

class FFLiBufferAllocator;

class FFLiCopySurfaceShader
{
public:
    FFLiCopySurfaceShader();
    ~FFLiCopySurfaceShader();

    bool SetupCPU(FFLiBufferAllocator* pAllocator);
    void SetupGPU();

    void Bind() const;

    void SetTexture(const agl::TextureData& textureData, u32 mipLevel);

private:
    rio::Shader         m_Shader;
    agl::TextureSampler m_Sampler;
    u32                 m_SamplerLocation;
};
//NN_STATIC_ASSERT(sizeof(FFLiCopySurfaceShader) == 0x3C);

#endif // FFLI_COPY_SURFACE_SHADER_H_
