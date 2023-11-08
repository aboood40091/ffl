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
    *ppTextureData = new agl::TextureData;

    u32 size = pResLoader->GetTextureAlignedMaxSize(partsType);
    void* pData;

    if (!pResLoader->IsExpand())
    {
        pData = rio::MemUtil::alloc(size, TEXTURE_DATA_MAX_ALIGNMENT);
        FFLResult result = pResLoader->LoadTexture(pData, &size, partsType, index);
        if (result != FFL_RESULT_OK)
        {
            delete *ppTextureData;
            *ppTextureData = nullptr;
            rio::MemUtil::free(pData);
            return result;
        }
    }
    else
    {
        pData = NULL;
        FFLResult result = pResLoader->GetPointerTextureByExpandCache(&pData, &size, partsType, index);
        if (result != FFL_RESULT_OK)
        {
            delete *ppTextureData;
            *ppTextureData = nullptr;
            return result;
        }
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
    if (mipPtr == nullptr && footer.NumMips() > 1)
        mipPtr = (void*)((uintptr_t)imagePtr + surface.mipOffset[0]);

#if RIO_IS_WIN
    surface.imagePtr = imagePtr;
    surface.mipPtr = mipPtr;

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

    (*ppTextureData)->initializeFromSurface(linearSurface);
#else
    if (!pResLoader->IsExpand())
    {
        surface.imagePtr = rio::MemUtil::alloc(surface.imageSize, surface.alignment);
        rio::MemUtil::copy(surface.imagePtr, imagePtr, surface.imageSize);

        if (surface.mipSize > 0)
        {
            surface.mipPtr = rio::MemUtil::alloc(surface.mipSize, surface.alignment);
            rio::MemUtil::copy(surface.mipPtr, mipPtr, surface.mipSize);
        }
        else
        {
            surface.mipPtr = nullptr;
        }
    }
    else
    {
        surface.imagePtr = imagePtr;
        surface.mipPtr = mipPtr;
    }

    (*ppTextureData)->initializeFromSurface(surface);
#endif // RIO_IS_WIN

  //if (!pResLoader->IsExpand())
  //    rio::MemUtil::free(pData);

    return FFL_RESULT_OK;
}

void FFLiDeleteTexture(agl::TextureData** ppTextureData, bool isExpand)
{
    agl::TextureData*& pTextureData = *ppTextureData;

#if RIO_IS_WIN
    u8* imagePtr = (u8*)pTextureData->getSurface().image;
    u8* mipPtr = (u8*)pTextureData->getSurface().mipmaps;

    delete[] imagePtr;
    if (pTextureData->getMipByteSize() > 0)
        delete[] mipPtr;
#else
    if (!isExpand)
    {
        void* imagePtr = pTextureData->getImagePtr();
        void* mipPtr = pTextureData->getMipPtr();

        rio::MemUtil::free(imagePtr);
        if (pTextureData->getMipByteSize() > 0)
            rio::MemUtil::free(mipPtr);
    }
#endif

    delete pTextureData;
    pTextureData = nullptr;
}
