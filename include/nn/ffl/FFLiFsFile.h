#ifndef FFLI_FS_FILE_H_
#define FFLI_FS_FILE_H_

#include <nn/ffl/types.h>

#include <cafe/fs.h>

class FFLiFsCommand;

class FFLiFsFile
{
public:
    FFLiFsFile(FFLiFsCommand* pCommand);
    ~FFLiFsFile();

    FSStatus Open(const char* path, const char* mode, FSRetFlag errHandling = FS_RET_NO_ERROR);
    FSStatus Close(FSRetFlag errHandling = FS_RET_NO_ERROR);
    
    FSStatus Read(void* dst, u32 size, u32 count, FSRetFlag errHandling = FS_RET_NO_ERROR, FSFlag flag = 0);
    FSStatus ReadWithPos(void* dst, u32 size, u32 count, u32 pos, FSRetFlag errHandling = FS_RET_NO_ERROR, FSFlag flag = 0);

    FSStatus Write(const void* src, u32 size, u32 count, FSRetFlag errHandling = FS_RET_NO_ERROR, FSFlag flag = 0);

    bool IsOpened() const;

    static FSStatus FlushQuota(FFLiFsCommand* pCommand, const char* path, FSRetFlag errHandling = FS_RET_NO_ERROR);

private:
    FFLiFsCommand*  m_pFsCommand;
    FSFileHandle    m_Handle;
};
NN_STATIC_ASSERT(sizeof(FFLiFsFile) == 8);

#endif // FFLI_FS_FILE_H_
