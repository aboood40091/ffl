#ifndef FFLI_DATABASE_DEFAULT_H_
#define FFLI_DATABASE_DEFAULT_H_

#include <nn/ffl/FFLiMiiDataCore.h>

struct FFLiCharInfo;

class FFLiDatabaseDefault
{
public:
    FFLiDatabaseDefault();
    ~FFLiDatabaseDefault();

private:
    static void InitElement(FFLiMiiDataOfficial* pMiiDataOfficial, const FFLiMiiDataCoreRFL* pMiiDataCoreRFL, u32 index);

public:
    bool Get(FFLiCharInfo* pCharInfo, u32 index) const;

private:
    FFLiMiiDataOfficial m_MiiDataOfficial[6];
};
NN_STATIC_ASSERT(sizeof(FFLiDatabaseDefault) == 0x228);

#endif // FFLI_DATABASE_DEFAULT_H_
