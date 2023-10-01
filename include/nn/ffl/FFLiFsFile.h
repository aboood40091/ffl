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

private:
    FFLiFsCommand*  m_pFsCommand;
    FSFileHandle    m_Handle;
};
NN_STATIC_ASSERT(sizeof(FFLiFsFile) == 8);

#endif // FFLI_FS_FILE_H_
