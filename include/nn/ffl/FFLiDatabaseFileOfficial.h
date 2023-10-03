#ifndef FFLI_DATABASE_FILE_OFFICIAL_H_
#define FFLI_DATABASE_FILE_OFFICIAL_H_

#include <nn/ffl/FFLCreateID.h>

#include <nn/ffl/FFLiMiiDataCore.h>

class FFLiDatabaseFileOfficial
{
public:
    struct AdjustRegularBuffer
    {
        u32         size;           // Count of used create IDs
        FFLCreateID createID[3000];
    };
    NN_STATIC_ASSERT(sizeof(AdjustRegularBuffer) == 0x7534);

public:
    void Init();

    bool IsValid() const;

    bool IsAvailable(u16 miiDataIndex, bool, bool) const;
    bool IsRegular(u16 miiDataIndex, bool, bool) const;

    bool AdjustRegularList(AdjustRegularBuffer* pBuffer);

private:
    u32                 m_Magic;
    u32                 _4;
    FFLiMiiDataOfficial m_MiiDataOfficial[3000];
    FFLCreateID         m_CreateID[50];
    u8                  _4381c[34];
    u16                 m_Crc;
};
NN_STATIC_ASSERT_IS_POD(FFLiDatabaseFileOfficial);
NN_STATIC_ASSERT(sizeof(FFLiDatabaseFileOfficial) == 0x43840);

#endif // FFLI_DATABASE_FILE_OFFICIAL_H_
