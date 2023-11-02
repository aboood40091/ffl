#include <nn/ffl/FFLiCreateID.h>
#include <nn/ffl/FFLiDatabaseFileOfficial.h>
#include <nn/ffl/FFLiMiiData.h>
#include <nn/ffl/FFLiSwapEndian.h>

#include <nn/ffl/detail/FFLiCharInfo.h>
#include <nn/ffl/detail/FFLiCrc.h>

#include <cstring>

static u32 GetMiiDataNum();

const FFLiMiiDataOfficial& FFLiDatabaseFileOfficial::GetImpl(u16 index) const
{
    return m_MiiDataOfficial[index];
}

bool FFLiDatabaseFileOfficial::FindRegularList(u32* pIndex, const FFLCreateID* pCreateID) const
{
    for (u32 i = 0; i < 50; i++)
    {
        if (FFLiIsSameMiiID(pCreateID, &(m_CreateID[i])))
        {
            if (pIndex != NULL)
                *pIndex = i;

            return true;
        }
    }

    return false;
}

void FFLiDatabaseFileOfficial::Init()
{
    m_Magic = 'FFOC';

    _4 = 0;

    std::memset(_4381c, 0, sizeof(_4381c));
    std::memset(m_MiiDataOfficial, 0, sizeof(m_MiiDataOfficial));
    std::memset(m_CreateID, 0, sizeof(m_CreateID));

    UpdateCrc();
}

void FFLiDatabaseFileOfficial::UpdateCrc()
{
    FFLiPutCRC16(0, this, sizeof(FFLiDatabaseFileOfficial));
}

bool FFLiDatabaseFileOfficial::IsValidCrc() const
{
    return FFLiIsValidCRC16(this, sizeof(FFLiDatabaseFileOfficial));
}

bool FFLiDatabaseFileOfficial::IsValidIdentifier() const
{
    return m_Magic == 'FFOC';
}

bool FFLiDatabaseFileOfficial::IsValid() const
{
    return IsValidIdentifier() && IsValidCrc();
}

static u32 GetMiiDataNum()
{
    return 3000;
}

bool FFLiDatabaseFileOfficial::Get(FFLiCharInfo* pCharInfo, u16 miiDataIndex, bool checkSpecial, bool allowSpecial) const
{
    if (miiDataIndex >= GetMiiDataNum())
        return false;

    const FFLiMiiDataOfficial& official = GetImpl(miiDataIndex);
    const FFLCreateID* pCreateID = &official.CreatorID();
    
    if (FFLiIsNullMiiID(pCreateID))
        return false;
    
    if (FFLiIsTemporaryMiiID(pCreateID))
        return false;

    if (checkSpecial)
    {
        if (!allowSpecial)
        {
            if (FFLiIsSpecialMiiID(pCreateID))
                return false;
        }
    }

    FFLiMiiDataOfficial2CharInfo(pCharInfo, official);
    if (!FFLiVerifyCharInfo(pCharInfo, true))
        return false;

    return true;
}

bool FFLiDatabaseFileOfficial::IsAvailable(u16 miiDataIndex, bool checkSpecial, bool allowSpecial) const
{
    FFLiCharInfo charInfo;
    return Get(&charInfo, miiDataIndex, checkSpecial, allowSpecial);
}

bool FFLiDatabaseFileOfficial::Search(u16* pMiiDataIndex, const FFLCreateID& createID) const
{
    for (u16 i = 0; i < GetMiiDataNum(); i++)
    {
        const FFLiMiiDataOfficial& official = GetImpl(i);
        const FFLCreateID* pCreateID = &official.CreatorID();

        if (FFLiIsSameMiiID(pCreateID, &createID))
        {
            FFLiCharInfo charInfo;
            if (Get(&charInfo, i, false, false))
            {
                if (pMiiDataIndex != NULL)
                    *pMiiDataIndex = i;

                return true;
            }
        }
    }

    return false;
}

bool FFLiDatabaseFileOfficial::IsRegular(u16 miiDataIndex, bool checkSpecial, bool allowSpecial) const
{
    FFLiCharInfo charInfo;
    if (!Get(&charInfo, miiDataIndex, checkSpecial, allowSpecial))
        return false;

    return FindRegularList(NULL, &charInfo.creatorID);
}

namespace {

bool IsExistCreateID(const FFLCreateID& createID, const FFLiDatabaseFileOfficial::AdjustRegularBuffer& buffer)
{
    for (u32 i = 0; i < buffer.size; i++)
        if (FFLiIsSameMiiID(&createID, &(buffer.createID[i])))
            return true;

    return false;
}

void SetNullCreateID(FFLCreateID& createID)
{
    std::memset(&createID, 0, sizeof(FFLCreateID));
}

}

bool FFLiDatabaseFileOfficial::AdjustRegularList(AdjustRegularBuffer* pBuffer)
{
    pBuffer->size = 0;

    for (u32 i = 0; i < 3000; i++)
    {
        FFLiCharInfo charInfo;
        if (Get(&charInfo, i, true, true))
            pBuffer->createID[pBuffer->size++] = charInfo.creatorID;
    }

    bool ret = false;

    for (u32 i = 0; i < 50; i++)
    {
        FFLCreateID& createID = m_CreateID[i];
        if (FFLiIsValidMiiID(&createID) && !IsExistCreateID(createID, *pBuffer))
        {
            SetNullCreateID(createID);
            ret = true;
        }
    }

    return ret;
}

void FFLiDatabaseFileOfficial::SwapEndian()
{
    // This function is deleted in NSMBU.
    // Therefore, its implementation is only theoretical.

    m_Magic = FFLiSwapEndianImpl<u32>(m_Magic);
    _4 = FFLiSwapEndianImpl<u32>(_4);

    for (u32 i = 0; i < 3000; i++)
        m_MiiDataOfficial[i].SwapEndian();

    // Dunno what to do with this
    // _4381c

    m_Crc = FFLiSwapEndianImpl<u16>(m_Crc);

    UpdateCrc();
}
