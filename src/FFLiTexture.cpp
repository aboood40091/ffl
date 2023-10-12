#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiTexture.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

#define TEXTURE_DATA_MAX_ALIGNMENT   (0x800)

u32 FFLiGetTextureMaxSizeWithAlign(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType, FFLiTexturePartsType partsType)
{
    u32 ret  = TEXTURE_DATA_MAX_ALIGNMENT;
    ret     += sizeof(GX2Texture);

    if (!pResourceManager->IsExpand(resourceType))
        ret += pResourceManager->GetTextureAlignedMaxSize(resourceType, partsType);

    return ret;
}

FFLResult FFLiLoadTextureWithAllocate(GX2Texture** ppGX2Texture, FFLiTexturePartsType partsType, u32 index, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator)
{
    u32 size = pResLoader->GetTextureAlignedMaxSize(partsType);

    void* pData = NULL;
    if (!pResLoader->IsExpand())
        pData = pAllocator->Allocate(size, TEXTURE_DATA_MAX_ALIGNMENT);

    *ppGX2Texture = static_cast<GX2Texture*>(pAllocator->Allocate(sizeof(GX2Texture), 4));

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

    GX2InitTexture(
        *ppGX2Texture,
        footer.Width(),
        footer.Height(),
        1,
        footer.NumMips(),
        footer.SurfaceFormat(),
        GX2_SURFACE_DIM_2D
    );
    GX2InitTexturePtrs(*ppGX2Texture, footer.GetImagePtrImpl(size), footer.GetMipPtrImpl(size));

    return FFL_RESULT_OK;
}

void FFLiInvalidateTexture(GX2Texture* pGX2Texture)
{
    if (pGX2Texture != NULL)
    {
        if (pGX2Texture->surface.imageSize != 0)
            FFLiInvalidate(GX2_INVALIDATE_CPU_TEXTURE, pGX2Texture->surface.imagePtr, pGX2Texture->surface.imageSize);

        if (pGX2Texture->surface.mipSize != 0)
            FFLiInvalidate(GX2_INVALIDATE_CPU_TEXTURE, pGX2Texture->surface.mipPtr, pGX2Texture->surface.mipSize);
    }
}
