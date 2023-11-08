#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiTexture.h>

#if RIO_IS_WIN
#include <ninTexUtils/gx2/gx2Surface.h>
#elif RIO_IS_CAFE
#define numMips mipLevels
#define imagePtr image
#define mipPtr mipmaps
#define mipOffset mipLevelOffset
#define GX2_SURFACE_DIM_2D GX2_SURFACE_DIM_TEXTURE_2D
#define GX2_AA_MODE_1X GX2_AA_MODE1X
#endif // RIO_IS_CAFE

#define TEXTURE_DATA_MAX_ALIGNMENT   (0x800)

FFLResult FFLiLoadTextureWithAllocate(rio::Texture2D** ppTexture2D, FFLiTexturePartsType partsType, u32 index, FFLiResourceLoader* pResLoader)
{
    u32 size = pResLoader->GetTextureAlignedMaxSize(partsType);
    void* pData;

    if (!pResLoader->IsExpand())
    {
        pData = rio::MemUtil::alloc(size, TEXTURE_DATA_MAX_ALIGNMENT);
        FFLResult result = pResLoader->LoadTexture(pData, &size, partsType, index);
        if (result != FFL_RESULT_OK)
        {
            rio::MemUtil::free(pData);
            return result;
        }
    }
    else
    {
        pData = NULL;
        FFLResult result = pResLoader->GetPointerTextureByExpandCache(&pData, &size, partsType, index);
        if (result != FFL_RESULT_OK)
            return result;
    }

    const FFLiResourceTextureFooter& footer = FFLiResourceTextureFooter::GetFooterImpl(pData, size);

    rio::NativeTexture2D texture;

#if RIO_IS_WIN
    GX2Surface surface;
#else
    GX2Surface& surface = texture.surface;
#endif
    surface.dim = GX2_SURFACE_DIM_2D;
    surface.width = footer.Width();
    surface.height = footer.Height();
    surface.depth = 1;
    surface.numMips = footer.NumMips();
    surface.format = footer.SurfaceFormat();
    surface.aa = GX2_AA_MODE_1X;
    surface.use = GX2_SURFACE_USE_TEXTURE;
    surface.tileMode = GX2_TILE_MODE_DEFAULT;
    surface.swizzle = 0;

    GX2CalcSurfaceSizeAndAlignment(&surface);

    void* imagePtr = footer.GetImagePtrImpl(size);
    RIO_ASSERT(imagePtr == pData);

    void* mipPtr = footer.GetMipPtrImpl(size);
    if (mipPtr == nullptr && footer.NumMips() > 1)
        mipPtr = (void*)((uintptr_t)imagePtr + surface.mipOffset[0]);

    surface.imagePtr = imagePtr;
    surface.mipPtr = mipPtr;

#if RIO_IS_WIN
    GX2Surface linearSurface;
    linearSurface.dim = GX2_SURFACE_DIM_2D;
    linearSurface.width = footer.Width();
    linearSurface.height = footer.Height();
    linearSurface.depth = 1;
    linearSurface.numMips = footer.NumMips();
    linearSurface.format = footer.SurfaceFormat();
    linearSurface.aa = GX2_AA_MODE_1X;
    linearSurface.use = GX2_SURFACE_USE_TEXTURE;
    linearSurface.tileMode = GX2_TILE_MODE_LINEAR_SPECIAL;
    linearSurface.swizzle = 0;

    GX2CalcSurfaceSizeAndAlignment(&linearSurface);

    linearSurface.imagePtr = new u8[linearSurface.imageSize];
    if (linearSurface.mipSize > 0)
        linearSurface.mipPtr = new u8[linearSurface.mipSize];
    else
        linearSurface.mipPtr = nullptr;

    GX2CopySurface(&surface, 0, 0, &linearSurface, 0, 0);

    for (u32 i = 1; i < footer.NumMips(); i++)
        GX2CopySurface(&surface, i, 0, &linearSurface, i, 0);

    if (!pResLoader->IsExpand())
        rio::MemUtil::free(pData);

    texture.surface.width = linearSurface.width;
    texture.surface.height = linearSurface.height;
    texture.surface.mipLevels = linearSurface.numMips;
    texture.surface.format = rio::TextureFormat(linearSurface.format);

    [[maybe_unused]] bool success = rio::TextureFormatUtil::getNativeTextureFormat(texture.surface.nativeFormat, texture.surface.format);
    RIO_ASSERT(success);

    texture.surface.imageSize = linearSurface.imageSize;
    texture.surface.mipmapSize = linearSurface.mipSize;
    texture.surface.mipLevelOffset[0] = 0;
    rio::MemUtil::copy(&texture.surface.mipLevelOffset[1], &linearSurface.mipOffset[1], sizeof(u32) * (13 - 1));
    texture.surface.image = linearSurface.imagePtr;
    texture.surface.mipmaps = linearSurface.mipPtr;

    texture.compMap = rio::TextureFormatUtil::getDefaultCompMap(texture.surface.format);

    {
        texture._footer.magic = 0x5101382D;
        texture._footer.version = 0x01000000;
    }
#else
    texture.viewFirstMip = 0;
    texture.viewNumMips = surface.mipLevels;
    texture.viewFirstSlice = 0;
    texture.viewNumSlices = surface.depth;
    texture.compMap = rio::TextureFormatUtil::getDefaultCompMap(rio::TextureFormat(surface.format));
    GX2InitTextureRegs(&texture);
#endif // RIO_IS_WIN

    *ppTexture2D = new rio::Texture2D(texture);

    return FFL_RESULT_OK;
}

void FFLiDeleteTexture(rio::Texture2D** ppTexture2D, bool isExpand)
{
    rio::Texture2D*& pTexture2D = *ppTexture2D;

#if RIO_IS_WIN
    u8* imagePtr = (u8*)pTexture2D->getNativeTexture().surface.image;
    u8* mipPtr = (u8*)pTexture2D->getNativeTexture().surface.mipmaps;

    delete[] imagePtr;
    if (pTexture2D->getNativeTexture().surface.mipmapSize > 0)
        delete[] mipPtr;
#else
    if (!isExpand)
    {
        void* pData = pTexture2D->getNativeTexture().surface.image;
        rio::MemUtil::free(pData);
    }
#endif

    delete pTexture2D;
    pTexture2D = nullptr;
}
