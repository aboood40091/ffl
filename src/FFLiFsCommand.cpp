#include <nn/ffl/FFLiFsCommand.h>

#include <new>

FFLiFsCommand::FFLiFsCommand(FFLiFsClient* pClient)
    : m_pFsClient(pClient)
{
    FSInitCmdBlock(&m_FSCmdBlock);
}

FFLiFsCommand::~FFLiFsCommand()
{
}

FFLiFsCommand* FFLiFsCommand::PlacementNew(FFLiFsCommandBuffer* pBuffer, FFLiFsClient* pClient)
{
    return new (pBuffer) FFLiFsCommand(pClient);
}

void FFLiFsCommand::PlacementDelete(FFLiFsCommand* pCommand)
{
    pCommand->~FFLiFsCommand();
}
