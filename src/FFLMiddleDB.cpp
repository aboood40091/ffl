#include <nn/ffl/FFLMiddleDB.h>

#include <nn/ffl/FFLiMiddleDB.h>

namespace {

FFLiMiddleDB* GetMiddleDB(FFLMiddleDB* pMiddleDB);
const FFLiMiddleDB* GetMiddleDB(const FFLMiddleDB* pMiddleDB);

}

u32 FFLGetMiddleDBBufferSize(u16 miiDataNum)
{
    return FFLiGetMiddleDBBufferSize(miiDataNum);
}

void FFLInitMiddleDB(FFLMiddleDB* pMiddleDB, FFLMiddleDBType type, void* pMiiData, u16 miiDataNum)
{
    FFLiInitMiddleDB(GetMiddleDB(pMiddleDB), type, pMiiData, miiDataNum);
}

FFLResult FFLUpdateMiddleDB(FFLMiddleDB* pMiddleDB)
{
    return FFLiUpdateMiddleDB(GetMiddleDB(pMiddleDB));
}

s32 FFLGetMiddleDBStoredSize(const FFLMiddleDB* pMiddleDB)
{
    return FFLiGetMiddleDBStoredSize(GetMiddleDB(pMiddleDB));
}

void FFLSetMiddleDBRandomMask(FFLMiddleDB* pMiddleDB, FFLGender gender, FFLAge age, FFLRace race)
{
    FFLiSetMiddleDBRandomMask(GetMiddleDB(pMiddleDB), gender, age, race);
}

void FFLSetMiddleDBHiddenMask(FFLMiddleDB* pMiddleDB, FFLGender gender)
{
    FFLiSetMiddleDBHiddenMask(GetMiddleDB(pMiddleDB), gender);
}

namespace {

FFLiMiddleDB* GetMiddleDB(FFLMiddleDB* pMiddleDB)
{
    return reinterpret_cast<FFLiMiddleDB*>(pMiddleDB);
}

const FFLiMiddleDB* GetMiddleDB(const FFLMiddleDB* pMiddleDB)
{
    return reinterpret_cast<const FFLiMiddleDB*>(pMiddleDB);
}

}
