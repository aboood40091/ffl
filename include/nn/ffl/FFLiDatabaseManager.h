#ifndef FFLI_DATABASE_MANAGER_H_
#define FFLI_DATABASE_MANAGER_H_

#include <nn/ffl/FFLDataSource.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiDatabaseDefault.h>
#include <nn/ffl/FFLiDatabaseFileAccessor.h>
#include <nn/ffl/FFLiDatabaseRandom.h>

class   FFLiSystemContext;

class FFLiDatabaseManager
{
public:
    FFLiDatabaseManager(FFLiDatabaseFile* pFile, FFLiFileWriteBuffer* pWriteBuffer, FFLiSystemContext* pContext, FFLiFsClient* pClient, FFLiAllocator* pAllocator);
    ~FFLiDatabaseManager();

    FFLResult GetStoreData(FFLiStoreDataCFL* pStoreDataCFL, FFLDataSource dataSource, u16 index);

private:
    FFLiSystemContext*          m_pSystemContext;
    FFLiDatabaseFileAccessor    m_DatabaseFileAccessor;
    FFLiDatabaseDefault         m_DatabaseDefault;
    FFLiDatabaseRandom          m_DatabaseRandom;
    bool                        m_IsEnabledSpecialMii;
    FFLiAllocator*              m_pAllocator;
};
NN_STATIC_ASSERT(sizeof(FFLiDatabaseManager) == 0xFD8);

#endif // FFLI_DATABASE_MANAGER_H_
