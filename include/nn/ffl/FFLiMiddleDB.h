#ifndef FFLI_MIDDLE_DB_H_
#define FFLI_MIDDLE_DB_H_

#include <nn/ffl/FFLGender.h>
#include <nn/ffl/FFLMiddleDBType.h>

#define FFLI_MIDDLE_DB_PARAM_SIZE   (4)

class FFLiMiddleDBHiddenParam
{
public:
    FFLGender Gender() const
    {
        return FFLGender(m_Gender);
    }

private:
    u8  m_Gender;
    u8  m_Padding[3];
};
NN_STATIC_ASSERT_IS_POD(FFLiMiddleDBHiddenParam);
NN_STATIC_ASSERT(sizeof(FFLiMiddleDBHiddenParam) == FFLI_MIDDLE_DB_PARAM_SIZE);

class FFLiMiddleDBRandomParam
{
private:
    u8  m_Gender;
    u8  m_Age;
    u8  m_Race;
    u8  m_Padding[1];
};
NN_STATIC_ASSERT_IS_POD(FFLiMiddleDBRandomParam);
NN_STATIC_ASSERT(sizeof(FFLiMiddleDBRandomParam) == FFLI_MIDDLE_DB_PARAM_SIZE);

class FFLiMiddleDBNetParam
{
private:
    u16 _0;
    u8  m_Padding[2];
};
NN_STATIC_ASSERT_IS_POD(FFLiMiddleDBNetParam);
NN_STATIC_ASSERT(sizeof(FFLiMiddleDBNetParam) == FFLI_MIDDLE_DB_PARAM_SIZE);

struct  FFLiCharInfo;
class   FFLiMiiDataHidden;
class   FFLiMiiDataOfficial;

class FFLiMiddleDB
{
public:
    u32 StoredSize() const;

    FFLMiddleDBType Type() const;

    void ClearData();

    const FFLiMiddleDBHiddenParam& HiddenParam() const;

    bool IsFull() const;

    void Add(const FFLiMiiDataHidden& miiDataHidden);

    FFLResult GetCharInfo(FFLiCharInfo* pCharInfo, u16 index) const;

private:
    u32                     m_Magic;
    FFLMiddleDBType         m_Type;
    FFLiMiiDataOfficial*    m_pMiiDataOfficial;
    u16                     m_Size;
    u16                     m_StoredSize;
    union
    {
        u8                      m_ParamData[FFLI_MIDDLE_DB_PARAM_SIZE];
        
        FFLiMiddleDBHiddenParam m_HiddenParam;
        FFLiMiddleDBRandomParam m_RandomParam;
        FFLiMiddleDBNetParam    m_NetParam;
    };
};
NN_STATIC_ASSERT_IS_POD(FFLiMiddleDB);
NN_STATIC_ASSERT(sizeof(FFLiMiddleDB) == 0x14);

#endif // FFLI_MIDDLE_DB_H_
