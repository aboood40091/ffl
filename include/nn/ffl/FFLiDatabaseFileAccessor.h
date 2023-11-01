#ifndef FFLI_DATABASE_FILE_ACCESSOR_H_
#define FFLI_DATABASE_FILE_ACCESSOR_H_

#include <nn/ffl/types.h>

class FFLiAllocator;
class FFLiDatabaseFile;
class FFLiFileWriteBuffer;

class FFLiDatabaseFileAccessor
{
public:
    FFLiDatabaseFileAccessor(FFLiDatabaseFile* pFile, FFLiFileWriteBuffer* pWriteBuffer, FFLiAllocator* pAllocator);
    ~FFLiDatabaseFileAccessor();

    FFLiDatabaseFile* GetDatabaseFile() const
    {
        return m_pDatabaseFile;
    }

    void Init();

    FFLResult AfterConstruct(u64 titleID);
    FFLResult BootLoad();
    FFLResult BootLoadImpl();
    FFLResult AdjustRegularListOfficial();

    FFLResult BeforeDestruct();

    FFLResult BeforeFlushQuota();
    FFLResult SaveHidden();
    FFLResult BackupOfficial();

    FFLResult FlushQuota(bool force);

    const char* GetPathOfficial() const;
    const char* GetPathBackup() const;
    const char* GetPathHidden() const;

    bool IsExistFile(const char* pPath);

private:
    void*                   _0;                         // Deleted
    u32                     _4[0xA84 / sizeof(u32)];    // Deleted
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
