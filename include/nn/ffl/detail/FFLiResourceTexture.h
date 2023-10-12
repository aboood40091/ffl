#ifndef FFLI_RESOURCE_TEXTURE_H_
#define FFLI_RESOURCE_TEXTURE_H_

#include <nn/ffl/FFLiTexturePartsType.h>

#include <nn/ffl/detail/FFLiResourcePartsInfo.h>

#include <cafe/gx2.h>

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

struct FFLiResourceTextureFooter
{
    s32 mipOffset;
    u16 width;
    u16 height;
    u8  numMips;
    u8  format;    // Maps to FFLiTextureFormat
};
NN_STATIC_ASSERT(sizeof(FFLiResourceTextureFooter) == 0xC);

#endif // FFLI_RESOURCE_TEXTURE_H_
