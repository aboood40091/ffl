#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiResourceUtil.h>

#include <nn/ffl/detail/FFLiResourceCache.h>

#include <cstring>

FFLiResourceCache::FFLiResourceCache()
{
    std::memset(m_Res, 0, sizeof(m_Res));
}

FFLiResourceCache::~FFLiResourceCache()
{
}

FFLResult FFLiResourceCache::Attach(void* pData, u32 size, FFLResourceType resourceType)
{
    RIO_ASSERT(pData != nullptr);
    RIO_ASSERT(size >= sizeof(FFLiResourceHeader));

    FFLiResourceHeader* pHeader = (FFLiResourceHeader*)pData;

#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
    pHeader->SwapEndian();
#endif // __BYTE_ORDER__

    FFLResult result = FFLiIsVaildResourceHeader(pHeader);
    if (result != FFL_RESULT_OK)
    {
#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
        pHeader->SwapEndian();
#endif // __BYTE_ORDER__
        return result;
    }

#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
    for (u32 i = 0; i < FFLI_TEXTURE_PARTS_TYPE_MAX; i++)
    {
        u32 num;
        FFLiResourcePartsInfo* pPartsInfo = FFLiGetTextureResoucePartsInfos(&num, pHeader->GetTextureHeader(), FFLiTexturePartsType(i));

        for (u32 j = 0; j < num; j++)
        {
            const FFLiResourcePartsInfo& partsInfo = pPartsInfo[j];
            u32 size = partsInfo.dataSize;
            if (size == 0 || partsInfo.strategy != FFLI_RESOURCE_STRATEGY_UNCOMPRESSED)
                continue;

            u8* pData = (u8*)pHeader + partsInfo.dataPos;

            FFLiResourceTextureFooter& footer = FFLiResourceTextureFooter::GetFooterImpl(pData, size);
            footer.SwapEndian();
        }
    }

    for (u32 i = 0; i < FFLI_SHAPE_PARTS_TYPE_MAX; i++)
    {
        u32 num;
        FFLiResourcePartsInfo* pPartsInfo = FFLiGetShapeResoucePartsInfos(&num, pHeader->GetShapeHeader(), FFLiShapePartsType(i));

        for (u32 j = 0; j < num; j++)
        {
            const FFLiResourcePartsInfo& partsInfo = pPartsInfo[j];
            u32 size = partsInfo.dataSize;
            if (size == 0 || partsInfo.strategy != FFLI_RESOURCE_STRATEGY_UNCOMPRESSED)
                continue;

            u8* pData = (u8*)pHeader + partsInfo.dataPos;

            FFLiSwapEndianResourceShapeElement(pData, FFLiShapePartsType(i), false);
        }
    }
#endif // __BYTE_ORDER__

    m_Res[resourceType].pHeader = pHeader;
    m_Res[resourceType].size = size;

    return FFL_RESULT_OK;
}

bool FFLiResourceCache::IsAttached() const
{
    for (u32 i = 0; i < FFL_RESOURCE_TYPE_MAX; i++)
        if (IsValid(FFLResourceType(i)))
            return true;

    return false;
}

bool FFLiResourceCache::IsValid(FFLResourceType resourceType) const
{
    return m_Res[resourceType].pHeader != NULL;
}

FFLiResourceHeader* FFLiResourceCache::Header(FFLResourceType resourceType) const
{
    return m_Res[resourceType].pHeader;
}
