#include <nn/ffl/FFLBirthPlatform.h>

#include <nn/ffl/FFLiCreateID.h>
#include <nn/ffl/FFLiDatabaseDefault.h>
#include <nn/ffl/FFLiMiiData.h>

static inline
FFLiCreateID* GetCreateID(FFLCreateID* pCreateID)
{
    return reinterpret_cast<FFLiCreateID*>(pCreateID);
}

static const FFLiMiiDataCoreRFL MII_DATA_CORE_RFL[6] = {
    {
        0x0008,
        { L'n', L'o', L' ', L'n', L'a', L'm', L'e' },
        64,
        64,
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0x1004,
        0x8800,
        { 0x3180, 0x08A2 },
        { 0x088C, 0x0858 },
        0x144A,
        0xB88D,
        0x008A,
        0x008A,
        0x2504
    },
    {
        0x000A,
        { L'n', L'o', L' ', L'n', L'a', L'm', L'e' },
        64,
        64,
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0x0004,
        0x6F80,
        { 0x3180, 0xC8A2 },
        { 0x088C, 0x8858 },
        0x144A,
        0xB88D,
        0x008A,
        0x008A,
        0x2504
    },
    {
        0x0000,
        { L'n', L'o', L' ', L'n', L'a', L'm', L'e' },
        64,
        64,
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0x0404,
        0x4240,
        { 0x3180, 0x28A2 },
        { 0x088C, 0x0858 },
        0x144A,
        0xB88D,
        0x008A,
        0x008A,
        0x2504
    },
    {
        0x4004,
        { L'n', L'o', L' ', L'n', L'a', L'm', L'e' },
        64,
        64,
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0x0804,
        0x3000,
        { 0x0180, 0x08A2 },
        { 0x106C, 0x0858 },
        0x144A,
        0xB88D,
        0x008A,
        0x008A,
        0x2504
    },
    {
        0x400C,
        { L'n', L'o', L' ', L'n', L'a', L'm', L'e' },
        64,
        64,
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0x0004,
        0x1DC0,
        { 0x0180, 0xE8A2 },
        { 0x106C, 0xA858 },
        0x144A,
        0xB88D,
        0x008A,
        0x008A,
        0x2504
    },
    {
        0x400E,
        { L'n', L'o', L' ', L'n', L'a', L'm', L'e' },
        64,
        64,
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        0x0004,
        0x1840,
        { 0x0180, 0x28A2 },
        { 0x106C, 0x0858 },
        0x144A,
        0xB88D,
        0x008A,
        0x008A,
        0x2504
    }
};

FFLiDatabaseDefault::FFLiDatabaseDefault()
{
    for (u32 i = 0; i < 6; i++)
        InitElement(&(m_MiiDataOfficial[i]), &(MII_DATA_CORE_RFL[i]), i);
}

FFLiDatabaseDefault::~FFLiDatabaseDefault()
{
}

namespace {

void GetDefaultCreateID(FFLCreateID* pCreateID, u32 index)
{
    static const FFLiCreateID defaultCreateID = {
        FFLI_CREATE_ID_FLAG_UNKNOWN_0 | FFLI_CREATE_ID_FLAG_UNKNOWN_2 | FFLI_CREATE_ID_FLAG_NORMAL,
        0,
        0,
        0,
        { 0xEC, 0xFF, 0x82, 0xD2, 0x00, 0x00 }
    };

    *GetCreateID(pCreateID) = defaultCreateID;
    GetCreateID(pCreateID)->databaseIndex = index;
}

}

void FFLiDatabaseDefault::InitElement(FFLiMiiDataOfficial* pMiiDataOfficial, const FFLiMiiDataCoreRFL* pMiiDataCoreRFL, u32 index)
{
    FFLiMiiDataCoreRFL2MiiDataCore(pMiiDataOfficial, *pMiiDataCoreRFL, true);
    GetDefaultCreateID(&pMiiDataOfficial->CreatorID(), index);
    FFLiClearCreatorNameFromOfficial(pMiiDataOfficial);
    pMiiDataOfficial->SetBirthPlatform(FFL_BIRTH_PLATFORM_WII_U);
    pMiiDataOfficial->SetCopyable(true);
}

static u32 GetMiiDataNum()
{
    return 6;
}

bool FFLiDatabaseDefault::Get(FFLiCharInfo* pCharInfo, u32 index) const
{
    if (index >= GetMiiDataNum())
        return false;

    FFLiMiiDataOfficial2CharInfo(pCharInfo, m_MiiDataOfficial[index]);
    return true;
}
