#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiMiddleDB.h>
#include <nn/ffl/FFLiMiiData.h>

#include <misc/rio_MemUtil.h>

u32 FFLiGetMiddleDBBufferSize(u16 miiDataNum)
{
    return FFLiMiddleDB::GetBufferSize(miiDataNum);
}

void FFLiInitMiddleDB(FFLiMiddleDB* pMiddleDB, FFLMiddleDBType type, void* pMiiData, u16 miiDataNum)
{
    pMiddleDB->Init(type, pMiiData, miiDataNum);
}

FFLResult FFLiUpdateMiddleDB(FFLiMiddleDB* pMiddleDB)
{
    if (!FFLiManager::IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    return FFLiManager::GetInstance()->GetDatabaseManager().UpdateMiddleDB(pMiddleDB);
}

void FFLiSetMiddleDBHiddenMask(FFLiMiddleDB* pMiddleDB, FFLGender gender)
{
    pMiddleDB->SetHiddenParam(gender);
}

void FFLiSetMiddleDBRandomMask(FFLiMiddleDB* pMiddleDB, FFLGender gender, FFLAge age, FFLRace race)
{
    pMiddleDB->SetRandomParam(gender, age, race);
}

s32 FFLiGetMiddleDBSize(const FFLiMiddleDB* pMiddleDB)
{
    return pMiddleDB->Size();
}

s32 FFLiGetMiddleDBStoredSize(const FFLiMiddleDB* pMiddleDB)
{
    return pMiddleDB->StoredSize();
}

FFLMiddleDBType FFLiGetMiddleDBType(const FFLiMiddleDB* pMiddleDB)
{
    return pMiddleDB->Type();
}

// --------------------------------------------------------------------------

void FFLiMiddleDBHiddenParam::Init()
{
    m_Gender = FFL_GENDER_MAX;
}

void FFLiMiddleDBHiddenParam::Set(FFLGender gender)
{
    m_Gender = gender;
}

void FFLiMiddleDBRandomParam::Init()
{
    m_Gender = FFL_GENDER_MAX;
    m_Age = FFL_AGE_MAX;
    m_Race = FFL_RACE_MAX;
}

void FFLiMiddleDBRandomParam::Set(FFLGender gender, FFLAge age, FFLRace race)
{
    m_Gender = gender;
    m_Age = age;
    m_Race = race;
}

void FFLiMiddleDBNetParam::Init()
{
    _0 = 0;
}

u32 FFLiMiddleDB::GetBufferSize(u16 num)
{
    return sizeof(FFLiMiiDataOfficial) * num;
}

void FFLiMiddleDB::Init(FFLMiddleDBType type, void* pData, u16 num)
{
    m_Magic = 0x46464D41;   // FFMA
    m_Type = type;
    m_pMiiDataOfficial = static_cast<FFLiMiiDataOfficial*>(pData);
    m_Size = num;
    m_StoredSize = 0;

    rio::MemUtil::set(m_ParamData, 0, FFLI_MIDDLE_DB_PARAM_SIZE);

    switch (type)
    {
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_RANDOM_UPDATE:
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE_REVERSE:
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE:
        m_HiddenParam.Init();
        break;
    case FFL_MIDDLE_DB_TYPE_RANDOM_PARAM:
        m_RandomParam.Init();
        break;
    case FFL_MIDDLE_DB_TYPE_NET_PARAM:
        m_NetParam.Init();
        break;
    }

    rio::MemUtil::set(m_pMiiDataOfficial, 0, FFLiGetMiddleDBBufferSize(m_Size));
}

void FFLiMiddleDB::SetHiddenParam(FFLGender gender)
{
    m_HiddenParam.Set(gender);
}

void FFLiMiddleDB::SetRandomParam(FFLGender gender, FFLAge age, FFLRace race)
{
    m_RandomParam.Set(gender, age, race);
}

s32 FFLiMiddleDB::Size() const
{
    return m_Size;
}

s32 FFLiMiddleDB::StoredSize() const
{
    return m_StoredSize;
}

FFLMiddleDBType FFLiMiddleDB::Type() const
{
    return m_Type;
}

void FFLiMiddleDB::ClearData()
{
    m_StoredSize = 0;
    rio::MemUtil::set(m_pMiiDataOfficial, 0, FFLiGetMiddleDBBufferSize(Size()));
}

const FFLiMiddleDBHiddenParam& FFLiMiddleDB::HiddenParam() const
{
    return m_HiddenParam;
}

const FFLiMiddleDBRandomParam& FFLiMiddleDB::RandomParam() const
{
    return m_RandomParam;
}

bool FFLiMiddleDB::IsFull() const
{
    if (StoredSize() < Size())
        return false;

    return true;
}

void FFLiMiddleDB::Add(const FFLiCharInfo& charInfo)
{
    FFLiCharInfo2MiiDataOfficial(&(m_pMiiDataOfficial[StoredSize()]), charInfo, false);
    m_StoredSize++;
}

void FFLiMiddleDB::Add(const FFLiMiiDataHidden& miiDataHidden)
{
    FFLiMiiDataOfficial& miiDataOfficial = m_pMiiDataOfficial[StoredSize()];
    static_cast<FFLiMiiDataCore&>(miiDataOfficial) = static_cast<const FFLiMiiDataCore&>(miiDataHidden);
    rio::MemUtil::set(miiDataOfficial.CreatorName(), 0, sizeof(u16) * 10);
    m_StoredSize++;
}

const FFLiMiiDataOfficial& FFLiMiddleDB::Get(u16 index) const
{
    return m_pMiiDataOfficial[index];
}

FFLiMiiDataOfficial& FFLiMiddleDB::Get(u16 index)
{
    return m_pMiiDataOfficial[index];
}

FFLResult FFLiMiddleDB::GetCharInfo(FFLiCharInfo* pCharInfo, u16 index) const
{
    if (index >= StoredSize())
        return FFL_RESULT_ERROR;

    FFLiMiiDataOfficial2CharInfo(pCharInfo, Get(index));
    return FFL_RESULT_OK;
}
