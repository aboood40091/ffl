#include <nn/ffl/FFLiFsClient.h>
#include <nn/ffl/FFLiFsCommand.h>
#include <nn/ffl/FFLiFsFile.h>

FFLiFsFile::FFLiFsFile(FFLiFsCommand* pCommand)
    : m_pFsCommand(pCommand)
    , m_Handle(FS_INVALID_HANDLE_VALUE)
{
}

FFLiFsFile::~FFLiFsFile()
{
}

FSStatus FFLiFsFile::Open(const char* path, const char* mode, FSRetFlag errHandling)
{
    return FSOpenFile(&m_pFsCommand->GetClient()->GetFSClient(), &m_pFsCommand->GetFSCmdBlock(), path, mode, &m_Handle, errHandling);
}

FSStatus FFLiFsFile::Close(FSRetFlag errHandling)
{
    FSStatus status = FSCloseFile(&m_pFsCommand->GetClient()->GetFSClient(), &m_pFsCommand->GetFSCmdBlock(), m_Handle, errHandling);
    if (status != FS_STATUS_OK)
        return status;

    m_Handle = FS_INVALID_HANDLE_VALUE;
    return FS_STATUS_OK;
}

FSStatus FFLiFsFile::Read(void* dst, u32 size, u32 count, FSRetFlag errHandling, FSFlag flag)
{
    return FSReadFile(&m_pFsCommand->GetClient()->GetFSClient(), &m_pFsCommand->GetFSCmdBlock(), (u8*)dst, size, count, m_Handle, flag, errHandling);
}

FSStatus FFLiFsFile::ReadWithPos(void* dst, u32 size, u32 count, u32 pos, FSRetFlag errHandling, FSFlag flag)
{
    return FSReadFileWithPos(&m_pFsCommand->GetClient()->GetFSClient(), &m_pFsCommand->GetFSCmdBlock(), (u8*)dst, size, count, pos, m_Handle, flag, errHandling);
}

FSStatus FFLiFsFile::Write(const void* src, u32 size, u32 count, FSRetFlag errHandling, FSFlag flag)
{
    return FSWriteFile(&m_pFsCommand->GetClient()->GetFSClient(), &m_pFsCommand->GetFSCmdBlock(), (u8*)src, size, count, m_Handle, flag, errHandling);
}

bool FFLiFsFile::IsOpened() const
{
    return m_Handle != FS_INVALID_HANDLE_VALUE;
}

FSStatus FFLiFsFile::FlushQuota(FFLiFsCommand* pCommand, const char* path, FSRetFlag errHandling)
{
    return FSFlushQuota(&pCommand->GetClient()->GetFSClient(), &pCommand->GetFSCmdBlock(), path, errHandling);
}
