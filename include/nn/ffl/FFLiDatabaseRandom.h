#ifndef FFLI_DATABASE_RANDOM_H_
#define FFLI_DATABASE_RANDOM_H_

#include <nn/ffl/types.h>

class FFLiMiddleDB;

class FFLiRandomContext;

class FFLiDatabaseRandom
{
public:
    FFLiDatabaseRandom(FFLiRandomContext* pContext);
    ~FFLiDatabaseRandom();

    FFLResult UpdateMiddleDB(FFLiMiddleDB* pMiddleDB);

private:
    FFLiRandomContext*  m_pRandomContext;
};
NN_STATIC_ASSERT(sizeof(FFLiDatabaseRandom) == 4);

#endif // FFLI_DATABASE_RANDOM_H_
