#ifndef FFLI_DATABASE_FILE_HIDDEN_H_
#define FFLI_DATABASE_FILE_HIDDEN_H_

#include <nn/ffl/FFLGender.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiMiiDataCore.h>

class FFLiOrderData
{
public:
    void Init();

    /*
    Deleted functions:

    void Clear();

    bool IsLinkedNext() const;
    bool IsLinkedPrev() const;
    bool IsUnlinked() const;

    void SetNextIndex(u16 index);
    void SetPrevIndex(u16 index);
    */

   u16 NextIndex() const;
   u16 PrevIndex() const;

   void SwapEndian();   // Deleted in NSMBU

private:
    u16 m_NextIndex;
    u16 m_PrevIndex;
};
NN_STATIC_ASSERT_IS_POD(FFLiOrderData);
NN_STATIC_ASSERT(sizeof(FFLiOrderData) == 4);

class FFLiAllocator;
class FFLiMiddleDB;

class FFLiDatabaseFileHidden
{
private:
    const FFLiMiiDataHidden& GetImpl(u16 index) const;

    u16 NumOfGenderWithIndex(u16* pIndices, FFLGender gender) const;

    FFLResult UpdateMiddleDBRandom(FFLiMiddleDB* pMiddleDB, FFLiAllocator* pAllocator) const;
    FFLResult UpdateMiddleDBTime(FFLiMiddleDB* pMiddleDB, bool reverse) const;

public:
    void Init();
    void UpdateCrc();

    bool IsValidCrc() const;
    bool IsValidIdentifier() const;
    bool IsValid() const;

    FFLResult UpdateMiddleDB(FFLiMiddleDB* pMiddleDB, FFLiAllocator* pAllocator) const;

    void SwapEndian();  // Deleted in NSMBU

private:
    u32                 m_Magic;
    u16                 m_StartIndex;
    u16                 m_EndIndex;
    FFLiOrderData       m_OrderData[500];
    FFLiMiiDataHidden   m_MiiDataHidden[500];
    u8                  _abe8[22];
    u16                 m_Crc;
};
NN_STATIC_ASSERT_IS_POD(FFLiDatabaseFileHidden);
NN_STATIC_ASSERT(sizeof(FFLiDatabaseFileHidden) == 0xAC00);

#endif // FFLI_DATABASE_FILE_HIDDEN_H_
