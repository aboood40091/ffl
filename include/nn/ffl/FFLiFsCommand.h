#ifndef FFLI_FS_COMMAND_H_
#define FFLI_FS_COMMAND_H_

#include <nn/ffl/types.h>

#include <cafe/fs.h>

class FFLiFsClient;

class FFLiFsCommand
{
public:
    FFLiFsCommand(FFLiFsClient* pClient);
    ~FFLiFsCommand();

private:
    FSCmdBlock      m_FSCmdBlock;
    FFLiFsClient*   m_pFsClient;
};
NN_STATIC_ASSERT(sizeof(FFLiFsCommand) == 0xA84);

#endif // FFLI_FS_COMMAND_H_
