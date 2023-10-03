#ifndef FFLI_DATABASE_FILE_HIDDEN_H_
#define FFLI_DATABASE_FILE_HIDDEN_H_

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

private:
    u16                 m_NextIndex;
    u16                 m_PrevIndex;
};
NN_STATIC_ASSERT_IS_POD(FFLiOrderData);
NN_STATIC_ASSERT(sizeof(FFLiOrderData) == 4);

class FFLiDatabaseFileHidden
{
public:
    void Init();
    void UpdateCrc();

    bool IsValid() const;

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
