#ifndef FFLI_RESOURCE_TEXTURE_H_
#define FFLI_RESOURCE_TEXTURE_H_

#include <nn/ffl/FFLiTexturePartsType.h>

#include <nn/ffl/detail/FFLiResourcePartsInfo.h>

#if RIO_IS_WIN
#include <ninTexUtils/gx2/gx2Enum.h>
#elif RIO_IS_CAFE
#include <gx2/enum.h>
#endif

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
    FFLiResourcePartsInfo   partsInfoCap[FFL_HAIR_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoEye[FFL_EYE_TYPE_TRUE_MAX];
    FFLiResourcePartsInfo   partsInfoEyebrow[FFL_EYEBROW_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoFaceline[FFL_FACE_LINE_MAX];
    FFLiResourcePartsInfo   partsInfoFaceMakeup[FFL_FACE_MAKE_MAX];
    FFLiResourcePartsInfo   partsInfoGlass[FFL_GLASS_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoMole[FFL_MOLE_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoMouth[FFL_MOUTH_TYPE_TRUE_MAX];
    FFLiResourcePartsInfo   partsInfoMustache[FFL_MUSTACHE_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoNoseline[FFL_NOSE_TYPE_MAX];
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

    void SwapEndian();  // Deleted in NSMBU

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
