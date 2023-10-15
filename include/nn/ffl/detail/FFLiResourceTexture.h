#ifndef FFLI_RESOURCE_TEXTURE_H_
#define FFLI_RESOURCE_TEXTURE_H_

#include <nn/ffl/FFLiTexturePartsType.h>

#include <nn/ffl/detail/FFLiResourcePartsInfo.h>

#include <cafe/gx2.h>

enum FFLiTextureFormat
{
    FFLI_TEXTURE_FORMAT_R8      = 0,
    FFLI_TEXTURE_FORMAT_RG8     = 1,
    FFLI_TEXTURE_FORMAT_RGBA8   = 2
};

GX2SurfaceFormat FFLiGetResourceSurfaceFormat(FFLiTextureFormat textureFormat);

// --------------------------------------------------------------------------

struct FFLiResourceTextureHeader
{
    u32                     partsMaxSize[FFLI_TEXTURE_PARTS_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoBeard[3];
    FFLiResourcePartsInfo   partsInfoCap[132];
    FFLiResourcePartsInfo   partsInfoEye[62];
    FFLiResourcePartsInfo   partsInfoEyebrow[24];
    FFLiResourcePartsInfo   partsInfoFaceline[12];
    FFLiResourcePartsInfo   partsInfoFaceMakeup[12];
    FFLiResourcePartsInfo   partsInfoGlass[9];
    FFLiResourcePartsInfo   partsInfoMole[2];
    FFLiResourcePartsInfo   partsInfoMouth[37];
    FFLiResourcePartsInfo   partsInfoMustache[6];
    FFLiResourcePartsInfo   partsInfoNoseline[18];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceTextureHeader) == 0x13FC);

class FFLiResourceTextureFooter
{
public:
    u16 Width() const
    {
        return m_Width;
    }

    u16 Height() const
    {
        return m_Height;
    }

    u8 NumMips() const
    {
        return m_NumMips;
    }

    GX2SurfaceFormat SurfaceFormat() const;

    static FFLiResourceTextureFooter& GetFooterImpl(const void* pData, u32 size);

    void* GetImagePtrImpl(u32 size) const;
    void* GetMipPtrImpl(u32 size) const;

private:
    u32 m_MipOffset;
    u16 m_Width;
    u16 m_Height;
    u8  m_NumMips;
    u8  m_TextureFormat;    // Maps to FFLiTextureFormat
};
NN_STATIC_ASSERT_IS_POD(FFLiResourceTextureFooter);
NN_STATIC_ASSERT(sizeof(FFLiResourceTextureFooter) == 0xC);

#endif // FFLI_RESOURCE_TEXTURE_H_
