#ifndef FFLI_DATABASE_FILE_ACCESSOR_H_
#define FFLI_DATABASE_FILE_ACCESSOR_H_

#include <nn/ffl/FFLiFsCommand.h>

class FFLiAllocator;
class FFLiDatabaseFile;
class FFLiFileWriteBuffer;
class FFLiFsClient;

class FFLiDatabaseFileAccessor
{
public:
    FFLiDatabaseFileAccessor(FFLiFsClient* pClient, FFLiDatabaseFile* pFile, FFLiFileWriteBuffer* pWriteBuffer, FFLiAllocator* pAllocator);
    ~FFLiDatabaseFileAccessor();

    FFLiDatabaseFile* GetDatabaseFile() const
    {
        return m_pDatabaseFile;
    }

private:
    FFLiFsClient*           m_pFsClient;
    FFLiFsCommand           m_FsCommand;
    FFLiDatabaseFile*       m_pDatabaseFile;
    FFLiFileWriteBuffer*    m_pFileWriteBuffer;
    FFLiAllocator*          m_pAllocator;
    u32                     _a94;               // Unused?
    char                    m_PathOfficial[FFL_PATH_MAX_LEN];
    char                    m_PathBackup[FFL_PATH_MAX_LEN];
    char                    m_PathHidden[FFL_PATH_MAX_LEN];
    bool                    m_IsPathSet;
    bool                    m_IsBackupOfficialNeed;
    bool                    m_IsHiddenSaved;
    bool                    m_IsFlushQuotaNeeded;
    u8                      _d9c;
    bool                    m_IsSaveHiddenNeeded;
};
NN_STATIC_ASSERT(sizeof(FFLiDatabaseFileAccessor) == 0xDA0);

#endif // FFLI_DATABASE_FILE_ACCESSOR_H_
