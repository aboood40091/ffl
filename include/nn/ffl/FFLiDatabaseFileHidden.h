#ifndef FFLI_DATABASE_FILE_HIDDEN_H_
#define FFLI_DATABASE_FILE_HIDDEN_H_

#include <nn/ffl/FFLiMiiDataCore.h>
#include <nn/ffl/FFLiOrderData.h>

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
