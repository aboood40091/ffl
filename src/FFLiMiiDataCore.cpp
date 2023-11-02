#include <nn/ffl/FFLiMiiDataCore.h>
#include <nn/ffl/FFLiSwapEndian.h>

#include <nn/ffl/detail/FFLiCrc.h>

static const FFLiSwapEndianDesc SWAP_ENDIAN_DESC[7] = {
    { FFLI_SWAP_ENDIAN_TYPE_U32,  1 },
    { FFLI_SWAP_ENDIAN_TYPE_U8,   8 },
    { FFLI_SWAP_ENDIAN_TYPE_U8,  10 },
    { FFLI_SWAP_ENDIAN_TYPE_U8,   2 },
    { FFLI_SWAP_ENDIAN_TYPE_U16, 11 },
    { FFLI_SWAP_ENDIAN_TYPE_U8,   2 },
    { FFLI_SWAP_ENDIAN_TYPE_U16, 12 }
};

void FFLiMiiDataCore::SwapEndian()
{
    FFLiSwapEndianGroup(this, SWAP_ENDIAN_DESC, 7);
}

void FFLiMiiDataOfficial::SwapEndian()
{
    FFLiMiiDataCore::SwapEndian();
    FFLiSwapEndianArrayImpl<u16>(m_CreatorName, 10);
}

void FFLiStoreData::SetCRC()
{
    FFLiPutCRC16(0, this, FFL_STOREDATA_SIZE);
}

void FFLiStoreData::SwapEndian()
{
    FFLiMiiDataOfficial::SwapEndian();
    _5c = FFLiSwapEndianImpl<u16>(_5c);
    m_Crc = FFLiSwapEndianImpl<u16>(m_Crc);
    SetCRC();
}

void FFLiMiiDataHidden::SwapEndian()
{
    // This function is deleted in NSMBU.
    // Therefore, its implementation is only theoretical.

    FFLiMiiDataCore::SwapEndian();

    // Dunno what to do with this
    // _48
}
