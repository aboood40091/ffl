#ifndef FFLI_DATABASE_MANAGER_H_
#define FFLI_DATABASE_MANAGER_H_

#include <nn/ffl/FFLiDatabaseDefault.h>
#include <nn/ffl/FFLiDatabaseFileAccessor.h>
#include <nn/ffl/FFLiDatabaseRandom.h>

class   FFLiSystemContext;

class FFLiDatabaseManager
{
public:
    FFLiDatabaseManager(FFLiDatabaseFile* pFile, FFLiFileWriteBuffer* pWriteBuffer, FFLiSystemContext* pContext, FFLiFsClient* pClient, FFLiAllocator* pAllocator);
    ~FFLiDatabaseManager();

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
