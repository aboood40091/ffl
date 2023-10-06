#ifndef FFLI_FS_COMMAND_H_
#define FFLI_FS_COMMAND_H_

#include <nn/ffl/types.h>

#include <cafe/fs.h>

#define FFLI_FS_COMMAND_SIZE    (0xA84)

struct FFLiFsCommandBuffer
{
    union
    {
        u8 data[FFLI_FS_COMMAND_SIZE];
        u32 value32[FFLI_FS_COMMAND_SIZE / sizeof(u32)];
    };
};
NN_STATIC_ASSERT(sizeof(FFLiFsCommandBuffer) == FFLI_FS_COMMAND_SIZE);

class FFLiFsClient;

class FFLiFsCommand
{
public:
    FFLiFsCommand(FFLiFsClient* pClient);
    ~FFLiFsCommand();

    static FFLiFsCommand* PlacementNew(FFLiFsCommandBuffer* pBuffer, FFLiFsClient* pClient);
    static void PlacementDelete(FFLiFsCommand* pCommand);

    FSCmdBlock& GetFSCmdBlock()
    {
        return m_FSCmdBlock;
    }

    const FSCmdBlock& GetFSCmdBlock() const
    {
        return m_FSCmdBlock;
    }

    FFLiFsClient* GetClient() const
    {
        return m_pFsClient;
    }

private:
    FSCmdBlock      m_FSCmdBlock;
    FFLiFsClient*   m_pFsClient;
};
NN_STATIC_ASSERT(sizeof(FFLiFsCommand) == FFLI_FS_COMMAND_SIZE);

#endif // FFLI_FS_COMMAND_H_
