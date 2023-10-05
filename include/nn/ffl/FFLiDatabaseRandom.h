#ifndef FFLI_DATABASE_RANDOM_H_
#define FFLI_DATABASE_RANDOM_H_

#include <nn/ffl/FFLResult.h>

enum FFLAge;
enum FFLGender;
enum FFLRace;

struct  FFLiCharInfo;
class   FFLiMiddleDB;
class   FFLiRandomContext;

class FFLiDatabaseRandom
{
public:
    FFLiDatabaseRandom(FFLiRandomContext* pContext);
    ~FFLiDatabaseRandom();

    FFLResult UpdateMiddleDB(FFLiMiddleDB* pMiddleDB);

private:
    void DetermineParam(FFLGender* pGender, FFLAge* pAge, FFLRace* pRace);
    void Get(FFLiCharInfo* pCharInfo, FFLGender gender, FFLAge age, FFLRace race);

private:
    FFLiRandomContext*  m_pRandomContext;
};
NN_STATIC_ASSERT(sizeof(FFLiDatabaseRandom) == 4);

#endif // FFLI_DATABASE_RANDOM_H_
