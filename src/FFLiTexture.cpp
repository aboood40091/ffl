#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiTexture.h>

#if RIO_IS_CAFE
#define GX2_SURFACE_DIM_2D GX2_SURFACE_DIM_TEXTURE_2D
#define GX2_AA_MODE_1X GX2_AA_MODE1X
#endif // RIO_IS_CAFE

#define TEXTURE_DATA_MAX_ALIGNMENT   (0x800)

FFLResult FFLiLoadTextureWithAllocate(agl::TextureData** ppTextureData, FFLiTexturePartsType partsType, u32 index, FFLiResourceLoader* pResLoader)
{
    u32 size = pResLoader->GetTextureAlignedMaxSize(partsType);

    void* pData = NULL;
    if (!pResLoader->IsExpand())
        pData = rio::MemUtil::alloc(size, TEXTURE_DATA_MAX_ALIGNMENT);

    *ppTextureData = new agl::TextureData;

    if (!pResLoader->IsExpand())
    {
        FFLResult result = pResLoader->LoadTexture(pData, &size, partsType, index);
        if (result != FFL_RESULT_OK)
            return result;
    }
    else
    {
        FFLResult result = pResLoader->GetPointerTextureByExpandCache(&pData, &size, partsType, index);
        if (result != FFL_RESULT_OK)
            return result;
    }

    const FFLiResourceTextureFooter& footer = FFLiResourceTextureFooter::GetFooterImpl(pData, size);

    GX2Surface surface;
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
    void* mipPtr = footer.GetMipPtrImpl(size);
    if (mipPtr == nullptr && surface.numMips > 1)
        mipPtr = (void*)((uintptr_t)imagePtr + surface.mipOffset[0]);

    surface.imagePtr = imagePtr;
    surface.mipPtr = mipPtr;

    (*ppTextureData)->initializeFromSurface(surface);

    return FFL_RESULT_OK;
}
