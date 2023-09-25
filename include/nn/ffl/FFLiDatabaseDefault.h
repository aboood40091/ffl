#ifndef FFLI_DATABASE_DEFAULT_H_
#define FFLI_DATABASE_DEFAULT_H_

#include <nn/ffl/FFLiMiiDataCore.h>

class FFLiDatabaseDefault
{
public:
    FFLiDatabaseDefault();
    ~FFLiDatabaseDefault();

private:
    FFLiMiiDataOfficial m_MiiDataOfficial[6];
};
NN_STATIC_ASSERT(sizeof(FFLiDatabaseDefault) == 0x228);

#endif // FFLI_DATABASE_DEFAULT_H_
