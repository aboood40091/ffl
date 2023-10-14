#include <nn/ffl/FFLiCreateID.h>
#include <nn/ffl/FFLiDatabaseFileHidden.h>
#include <nn/ffl/FFLiDateTime.h>
#include <nn/ffl/FFLiMiddleDB.h>

#include <nn/ffl/detail/FFLiAllocator.h>
#include <nn/ffl/detail/FFLiCrc.h>

#include <algorithm>
#include <cstring>

void FFLiOrderData::Init()
{
    m_NextIndex = 0xffff;
    m_PrevIndex = 0xffff;
}

u16 FFLiOrderData::NextIndex() const
{
    return m_NextIndex;
}

u16 FFLiOrderData::PrevIndex() const
{
    return m_PrevIndex;
}

static u32 GetMiiDataNum();

const FFLiMiiDataHidden& FFLiDatabaseFileHidden::GetImpl(u16 index) const
{
    return m_MiiDataHidden[index];
}

u16 FFLiDatabaseFileHidden::NumOfGenderWithIndex(u16* pIndices, FFLGender gender) const
{
    u16 num = 0;

    for (u16 i = 0; i < GetMiiDataNum(); i++)
    {
        const FFLiMiiDataHidden& hidden = GetImpl(i);
        if (!FFLiIsNullMiiID(&hidden.CreatorID()))
        {
            if (gender == FFL_GENDER_MAX || gender == hidden.Gender())
            {
                if (pIndices != NULL)
                    pIndices[num] = i;

                num++;
            }
        }
    }

    return num;
}

namespace {

void ShuffleIndices(u16* pIndices, u16 num)
{
    u32 holdrand = FFLiGetTick();

    for (u16 i = 0; i < num - 1; i++)
    {
        u32 range = num - i;
        u32 random = holdrand >> 16 & 0xffff;
        u16 iRandom = i + (random - (random / range) * range);
        holdrand = holdrand * 83409763L + 1444837343L;
        
        if (iRandom != i)
            std::swap(pIndices[iRandom], pIndices[i]);
    }
}

}

FFLResult FFLiDatabaseFileHidden::UpdateMiddleDBRandom(FFLiMiddleDB* pMiddleDB, FFLiAllocator* pAllocator) const
{
    u16* pIndices = static_cast<u16*>(pAllocator->Allocate(sizeof(u16) * GetMiiDataNum()));
    if (pIndices == NULL)
        return FFL_RESULT_OUT_OF_MEMORY;

    u16 num = NumOfGenderWithIndex(pIndices, pMiddleDB->HiddenParam().Gender());
    if (num == 0)
    {
        pAllocator->Free(pIndices);
        return FFL_RESULT_HDB_EMPTY;
    }

    ShuffleIndices(pIndices, num);

    if (!pMiddleDB->IsFull())
        for (u16 i = 0; i < num; i++)
            pMiddleDB->Add(GetImpl(pIndices[i]));

    pAllocator->Free(pIndices);
    return FFL_RESULT_OK;
}

struct FFLiMiddleDBTimeUpdateParam
{
    const FFLiOrderData*        pOrderData;
    const FFLiMiiDataHidden*    pMiiDataHidden;
    FFLGender                   gender;
    u16                         index;
    bool                        forward;
};

static bool ParamIsInvalid(const FFLiMiddleDBTimeUpdateParam& param)
{
    return param.index == 0xffff;
}

static const FFLiMiiDataHidden& ParamGetHidden(const FFLiMiddleDBTimeUpdateParam& param)
{
    return param.pMiiDataHidden[param.index];
}

static void ParamAdvance(FFLiMiddleDBTimeUpdateParam& param)
{
    if (param.forward)
        param.index = param.pOrderData[param.index].NextIndex();

    else
        param.index = param.pOrderData[param.index].PrevIndex();
}

static void ParamFindNext(FFLiMiddleDBTimeUpdateParam& param)
{
    if (param.index == 0xffff)
        return;

    while (true)
    {
        ParamAdvance(param);
        
        if (param.index == 0xffff)
            break;
        
        if (param.gender == FFL_GENDER_MAX)
            break;

        if (param.gender == ParamGetHidden(param).Gender())
            return;
    }
}

FFLResult FFLiDatabaseFileHidden::UpdateMiddleDBTime(FFLiMiddleDB* pMiddleDB, bool reverse) const
{
    FFLGender gender = pMiddleDB->HiddenParam().Gender();

    FFLiMiddleDBTimeUpdateParam param;
    param.pOrderData = m_OrderData;
    param.pMiiDataHidden = m_MiiDataHidden;
    param.gender = gender;
    param.index = reverse ? m_EndIndex : m_StartIndex;
    param.forward = !reverse;

    if (!ParamIsInvalid(param) && gender != FFL_GENDER_MAX && gender != ParamGetHidden(param).Gender())
        ParamFindNext(param);

    while (!pMiddleDB->IsFull() && param.index != 0xffff)
    {
        pMiddleDB->Add(GetImpl(param.index));
        ParamFindNext(param);
    }
    
    return
        pMiddleDB->StoredSize() != 0
            ? FFL_RESULT_OK
            : FFL_RESULT_HDB_EMPTY;
}

void FFLiDatabaseFileHidden::Init()
{
    m_Magic = 'FFHA';

    m_StartIndex = 0xffff;
    m_EndIndex = 0xffff;

    for (u32 i = 0; i < 500; i++)
        m_OrderData[i].Init();

    std::memset(m_MiiDataHidden, 0, sizeof(m_MiiDataHidden));
    std::memset(_abe8, 0, sizeof(_abe8));

    UpdateCrc();
}

void FFLiDatabaseFileHidden::UpdateCrc()
{
    FFLiPutCRC16(0, this, sizeof(FFLiDatabaseFileHidden));
}

bool FFLiDatabaseFileHidden::IsValidCrc() const
{
    return FFLiIsValidCRC16(this, sizeof(FFLiDatabaseFileHidden));
}

bool FFLiDatabaseFileHidden::IsValidIdentifier() const
{
    return m_Magic == 'FFHA';
}

bool FFLiDatabaseFileHidden::IsValid() const
{
    return IsValidIdentifier() && IsValidCrc();
}

FFLResult FFLiDatabaseFileHidden::UpdateMiddleDB(FFLiMiddleDB* pMiddleDB, FFLiAllocator* pAllocator) const
{
    FFLResult result;

    switch (pMiddleDB->Type())
    {
    default:
        result = FFL_RESULT_ERROR;
        break;
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_RANDOM_UPDATE:
        result = UpdateMiddleDBRandom(pMiddleDB, pAllocator);
        break;
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE_REVERSE:
        result = UpdateMiddleDBTime(pMiddleDB, true);
        break;
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE:
        result = UpdateMiddleDBTime(pMiddleDB, false);
        break;
    }

    return result;
}

static u32 GetMiiDataNum()
{
    return 500;
}
