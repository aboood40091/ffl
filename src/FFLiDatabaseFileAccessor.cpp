#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiDatabaseFile.h>
#include <nn/ffl/FFLiDatabaseFileAccessor.h>
#include <nn/ffl/FFLiPath.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiFileWriteBuffer.h>

#include <filedevice/rio_FileDeviceMgr.h>

enum FFLiFsFileResult
{
    FFLI_FS_FILE_RESULT_OK                  = 0,
    FFLI_FS_FILE_RESULT_READ_BUFFER_EMPTY   = 1,
    FFLI_FS_FILE_RESULT_WRITE_BUFFER_EMPTY  = 2,
    FFLI_FS_FILE_RESULT_OUT_OF_MEMORY       = 3,
    FFLI_FS_FILE_RESULT_NOT_FOUND           = 4
};

namespace {

struct FileResult
{
    FFLiFsFileResult    fileResult;
    rio::RawErrorCode   fsStatus;

    bool IsSuccess() const;
    FFLResult ReturnResult(FFLResult result) const;
    static bool IsRunMiiStudioStatus(rio::RawErrorCode status);
};
NN_STATIC_ASSERT(sizeof(FileResult) == 8);

const char* ODB_FILE_NAME[1] = {
    "db/FFL_ODB.dat"
};

const char* BACKUP_FILE_NAME[1] = {
    "db/FFL_ODB_OLD.dat"
};

const char* HDB_FILE_NAME[1] = {
    "db/FFL_HDB.dat"
};

FFLResult GetDatabasePath(char* pDst, u32 size, u64 titleID, const char* filename);

FileResult LoadDatabaseHidden(FFLiDatabaseFileHidden* pHidden, const char* pPath);
FileResult SaveDatabaseHidden(const FFLiDatabaseFileHidden& hidden, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath);

FileResult LoadDatabaseOfficial(FFLiDatabaseFileOfficial* pOfficial, const char* pPath);
FileResult SaveDatabaseOfficial(const FFLiDatabaseFileOfficial& official, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath);

FileResult CopyDatabaseOfficial(const char* pPathTo, const char* pPathFrom, FFLiFileWriteBuffer* pWriteBuffer);

}

FFLiDatabaseFileAccessor::FFLiDatabaseFileAccessor(FFLiDatabaseFile* pFile, FFLiFileWriteBuffer* pWriteBuffer)
    : m_pDatabaseFile(pFile)
    , m_pFileWriteBuffer(pWriteBuffer)
    , _a94(0)
    , m_IsPathSet(false)
    , m_IsBackupOfficialNeed(false)
    , m_IsHiddenSaved(false)
  //, m_IsFlushQuotaNeeded(false)
    , _d9c(0)
    , m_IsSaveHiddenNeeded(false)
{
    rio::MemUtil::set(m_PathOfficial, 0, FFL_PATH_MAX_LEN);
    rio::MemUtil::set(m_PathBackup,   0, FFL_PATH_MAX_LEN);
    rio::MemUtil::set(m_PathHidden,   0, FFL_PATH_MAX_LEN);
}

FFLiDatabaseFileAccessor::~FFLiDatabaseFileAccessor()
{
}

void FFLiDatabaseFileAccessor::Init()
{
    m_pDatabaseFile->official.Init();
    m_pDatabaseFile->hidden.Init();
}

FFLResult FFLiDatabaseFileAccessor::AfterConstruct(u64 titleID)
{
    FFLResult result = GetDatabasePath(m_PathOfficial, FFL_PATH_MAX_LEN, titleID, ODB_FILE_NAME[0]);
    if (result != FFL_RESULT_OK)
        return result;

    result = GetDatabasePath(m_PathBackup, FFL_PATH_MAX_LEN, titleID, BACKUP_FILE_NAME[0]);
    if (result != FFL_RESULT_OK)
        return result;

    result = GetDatabasePath(m_PathHidden, FFL_PATH_MAX_LEN, titleID, HDB_FILE_NAME[0]);
    if (result != FFL_RESULT_OK)
        return result;

    m_IsPathSet = true;
    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseFileAccessor::BootLoad()
{
    FFLResult result = BootLoadImpl();
    if (result != FFL_RESULT_OK)
        return result;

    result = AdjustRegularListOfficial();
    if (result != FFL_RESULT_OK)
        return result;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseFileAccessor::BootLoadImpl()
{
    bool needInitHidden = true;

    FileResult result = LoadDatabaseHidden(&m_pDatabaseFile->hidden, GetPathHidden());
    if (result.IsSuccess())
    {
        if (m_pDatabaseFile->hidden.IsValid())
            needInitHidden = false;
    }
    else if (result.fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
    {
        return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);
    }

    if (needInitHidden)
    {
        m_pDatabaseFile->hidden.Init();
        m_IsFlushQuotaNeeded = true;

        result = SaveDatabaseHidden(m_pDatabaseFile->hidden, m_pFileWriteBuffer, GetPathHidden());
        if (!result.IsSuccess())
            return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);
    }

    result = LoadDatabaseOfficial(&m_pDatabaseFile->official, GetPathOfficial());
    if (result.IsSuccess())
    {
        if (m_pDatabaseFile->official.IsValid())
        {
            if (!IsExistFile(GetPathBackup()))
            {
                m_IsFlushQuotaNeeded = true;

                result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathBackup());
                if (!result.IsSuccess())
                    return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);
            }

            return FFL_RESULT_OK;
        }
    }
    else if (result.fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
    {
        return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);
    }

    result = LoadDatabaseOfficial(&m_pDatabaseFile->official, GetPathBackup());
    if (result.IsSuccess())
    {
        if (m_pDatabaseFile->official.IsValid())
        {
            m_IsFlushQuotaNeeded = true;

            result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathOfficial());
            if (!result.IsSuccess())
                return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);

            return FFL_RESULT_OK;
        }
    }
    else if (result.fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
    {
        return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);
    }

    Init();

    m_IsFlushQuotaNeeded = true;

    result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathBackup());
    if (!result.IsSuccess())
        return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);

    result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathOfficial());
    if (!result.IsSuccess())
        return result.ReturnResult(FFL_RESULT_FILE_LOAD_ERROR);

    return FFL_RESULT_ODB_EMPTY;
}

FFLResult FFLiDatabaseFileAccessor::AdjustRegularListOfficial()
{
    FFLiDatabaseFileOfficial::AdjustRegularBuffer* pBuffer = new FFLiDatabaseFileOfficial::AdjustRegularBuffer;
    if (pBuffer == NULL)
        return FFL_RESULT_OUT_OF_MEMORY;

    m_pDatabaseFile->official.AdjustRegularList(pBuffer);

    delete pBuffer;
    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseFileAccessor::BeforeDestruct()
{
    if (!m_IsPathSet)
        return FFL_RESULT_FILE_LOAD_ERROR;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseFileAccessor::BeforeFlushQuota()
{
    if (m_IsSaveHiddenNeeded)
    {
        FFLResult result = SaveHidden();
        if (result != FFL_RESULT_OK)
            return result;
    }

    if (m_IsBackupOfficialNeed)
    {
        FFLResult result = BackupOfficial();
        if (result != FFL_RESULT_OK)
            return result;
    }

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseFileAccessor::SaveHidden()
{
    m_pDatabaseFile->hidden.UpdateCrc();

    FileResult result = SaveDatabaseHidden(m_pDatabaseFile->hidden, m_pFileWriteBuffer, GetPathHidden());
    if (!result.IsSuccess())
        return result.ReturnResult(FFL_RESULT_FILE_SAVE_ERROR);

    m_IsSaveHiddenNeeded = false;
    m_IsHiddenSaved = true;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseFileAccessor::BackupOfficial()
{
    const char* pPathFrom   = GetPathOfficial();
    const char* pPathTo     = GetPathBackup();

    FileResult result = CopyDatabaseOfficial(pPathTo, pPathFrom, m_pFileWriteBuffer);
    if (!result.IsSuccess())
        return result.ReturnResult(FFL_RESULT_FILE_SAVE_ERROR);

    return FFL_RESULT_OK;
}

#if RIO_IS_CAFE
extern "C" FSStatus FSFlushQuota(FSClient *client, FSCmdBlock* block, const char* path, FSErrorFlag errorMask);
#endif // RIO_IS_CAFE

FFLResult FFLiDatabaseFileAccessor::FlushQuota(bool force)
{
    u8 flush = m_IsFlushQuotaNeeded || m_IsBackupOfficialNeed || m_IsHiddenSaved;

    if (force | flush)
    {
#if RIO_IS_CAFE
        FSCmdBlock block;
        FSInitCmdBlock(&block);

        FSClient* client = rio::FileDeviceMgr::instance()->getFSClient();
        if (FSFlushQuota(client, &block, m_PathOfficial, FS_ERROR_FLAG_NONE) != FS_STATUS_OK)
            return FFL_RESULT_FS_ERROR;
#endif // RIO_IS_CAFE

        m_IsBackupOfficialNeed = false;
        m_IsHiddenSaved = false;
        m_IsFlushQuotaNeeded = false;
    }

    return FFL_RESULT_OK;
}

const char* FFLiDatabaseFileAccessor::GetPathOfficial() const
{
    return m_PathOfficial;
}

const char* FFLiDatabaseFileAccessor::GetPathBackup() const
{
    return m_PathBackup;
}

const char* FFLiDatabaseFileAccessor::GetPathHidden() const
{
    return m_PathHidden;
}

bool FFLiDatabaseFileAccessor::IsExistFile(const char* pPath)
{
    bool isExist = false;
    return rio::FileDeviceMgr::instance()->getNativeFileDevice()->tryIsExistFile(&isExist, pPath) && isExist;
}

bool FileResult::IsSuccess() const
{
    if (fileResult != FFLI_FS_FILE_RESULT_OK)
        return false;

    if (fsStatus != rio::RAW_ERROR_OK)
        return false;

    return true;
}

FFLResult FileResult::ReturnResult(FFLResult result) const
{
    if (fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
        return FFL_RESULT_FS_NOT_FOUND;

    if (fileResult == FFLI_FS_FILE_RESULT_OUT_OF_MEMORY)
        return FFL_RESULT_OUT_OF_MEMORY;

    return result;
}

bool FileResult::IsRunMiiStudioStatus(rio::RawErrorCode status)
{
    if (status == rio::RAW_ERROR_NOT_FOUND)
        return true;

    if (status == rio::RAW_ERROR_PERMISSION_ERROR)
        return true;

    return false;
}


namespace {

FFLResult GetDatabasePath(char* pDst, u32 size, u64 titleID, const char* filename)
{
    rio::RawErrorCode status = FFLiGetDatabasePath(pDst, size, titleID, filename);
    return FFLiConvertFSStatusToFFLResult(status);
}

FileResult ReadFile(void* pDst, u32 size, const char* pPath)
{
    rio::NativeFileDevice* device = rio::FileDeviceMgr::instance()->getNativeFileDevice();

    rio::FileHandle fileHandle;
    if (!device->tryOpen(&fileHandle, pPath, rio::FileDevice::FILE_OPEN_FLAG_READ))
    {
        rio::RawErrorCode status = device->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return FileResult { FFLI_FS_FILE_RESULT_OK, status };
    }

    u32 readSize = 0;
    if (!fileHandle.tryRead(&readSize, static_cast<u8*>(pDst), size))
    {
        fileHandle.tryClose();

        rio::RawErrorCode status = device->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return FileResult { FFLI_FS_FILE_RESULT_OK, status };
    }

    if (readSize == 0)
    {
        fileHandle.tryClose();

        return FileResult { FFLI_FS_FILE_RESULT_READ_BUFFER_EMPTY };
    }
    else
    {
        if (!fileHandle.tryClose())
        {
            rio::RawErrorCode status = device->getLastRawError();
            RIO_ASSERT(status != rio::RAW_ERROR_OK);
            return FileResult { FFLI_FS_FILE_RESULT_OK, status };
        }
    }

    return FileResult { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
}

rio::RawErrorCode WriteFileImpl(rio::FileHandle& fileHandle, const void* pSrc, u32 size, FFLiFileWriteBuffer* pWriteBuffer)
{
    rio::RawErrorCode ret = rio::RawErrorCode(0);

    const u32 count = (size + (FFLI_FILE_WRITE_BUFFER_SIZE - 1)) / FFLI_FILE_WRITE_BUFFER_SIZE;

    for (u32 i = 0; i < count; i++)
    {
        u32 writeSize = FFLiMin(size - i * FFLI_FILE_WRITE_BUFFER_SIZE, FFLI_FILE_WRITE_BUFFER_SIZE);
        rio::MemUtil::copy(pWriteBuffer, ((const u8*)pSrc) + i * FFLI_FILE_WRITE_BUFFER_SIZE, writeSize);

        u32 writtenSize = 0;
        if (fileHandle.tryWrite(&writtenSize, pWriteBuffer->data, writeSize) && writtenSize > 0)
            ret = rio::RawErrorCode(1);

        else
        {
            ret = fileHandle.getDevice()->getLastRawError();
            break;
        }
    }

    return ret;
}

FileResult WriteFile(const void* pSrc, u32 size, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath)
{
    rio::NativeFileDevice* device = rio::FileDeviceMgr::instance()->getNativeFileDevice();

    rio::FileHandle fileHandle;
    if (!device->tryOpen(&fileHandle, pPath, rio::FileDevice::FILE_OPEN_FLAG_WRITE))
    {
        rio::RawErrorCode status = device->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        if (FileResult::IsRunMiiStudioStatus(status))
            return FileResult { FFLI_FS_FILE_RESULT_NOT_FOUND };
        else
            return FileResult { FFLI_FS_FILE_RESULT_OK, status };
    }

    rio::RawErrorCode writeStatus = WriteFileImpl(fileHandle, pSrc, size, pWriteBuffer);
    if (writeStatus == 0)
    {
        fileHandle.tryClose();

        return FileResult { FFLI_FS_FILE_RESULT_WRITE_BUFFER_EMPTY };
    }
    else if (writeStatus < 0)
    {
        fileHandle.tryClose();

        return FileResult { FFLI_FS_FILE_RESULT_OK, writeStatus };
    }
    else
    {
        if (!fileHandle.tryClose())
        {
            rio::RawErrorCode status = device->getLastRawError();
            RIO_ASSERT(status < 0);
            return FileResult { FFLI_FS_FILE_RESULT_OK, status };
        }
    }

    return FileResult { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
}

FileResult LoadDatabaseHidden(FFLiDatabaseFileHidden* pHidden, const char* pPath)
{
    FileResult result = ReadFile(pHidden, sizeof(FFLiDatabaseFileHidden), pPath);
    if (result.IsSuccess())
    {
#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
        pHidden->SwapEndian(false);
#endif // __BYTE_ORDER__
        result = { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
    }
    return result; 
}

FileResult SaveDatabaseHidden(const FFLiDatabaseFileHidden& hidden, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath)
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return WriteFile(&hidden, sizeof(FFLiDatabaseFileHidden), pWriteBuffer, pPath);
#else
    FFLiDatabaseFileHidden* pHiddenBE = new FFLiDatabaseFileHidden(hidden);
    pHiddenBE->SwapEndian(true);
    FileResult result = WriteFile(pHiddenBE, sizeof(FFLiDatabaseFileHidden), pWriteBuffer, pPath);
    delete pHiddenBE;
    return result;
#endif // __BYTE_ORDER__
}

FileResult LoadDatabaseOfficial(FFLiDatabaseFileOfficial* pOfficial, const char* pPath)
{
    FileResult result = ReadFile(pOfficial, sizeof(FFLiDatabaseFileOfficial), pPath);
    if (result.IsSuccess())
    {
#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
        pOfficial->SwapEndian(false);
#endif // __BYTE_ORDER__
        result = { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
    }
    return result; 
}

FileResult SaveDatabaseOfficial(const FFLiDatabaseFileOfficial& official, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath)
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return WriteFile(&official, sizeof(FFLiDatabaseFileOfficial), pWriteBuffer, pPath);
#else
    FFLiDatabaseFileOfficial* pOfficialBE = new FFLiDatabaseFileOfficial(official);
    pOfficialBE->SwapEndian(true);
    FileResult result = WriteFile(pOfficialBE, sizeof(FFLiDatabaseFileOfficial), pWriteBuffer, pPath);
    delete pOfficialBE;
    return result;
#endif // __BYTE_ORDER__
}

FileResult CopyDatabaseOfficial(const char* pPathTo, const char* pPathFrom, FFLiFileWriteBuffer* pWriteBuffer)
{
    FFLiDatabaseFileOfficial* pOfficial = static_cast<FFLiDatabaseFileOfficial*>(rio::MemUtil::alloc(sizeof(FFLiDatabaseFileOfficial), rio::FileDevice::cBufferMinAlignment));
    if (pOfficial == NULL)
        return FileResult { FFLI_FS_FILE_RESULT_OUT_OF_MEMORY };

    FileResult result = LoadDatabaseOfficial(pOfficial, pPathFrom);
    if (result.IsSuccess())
        result = SaveDatabaseOfficial(*pOfficial, pWriteBuffer, pPathTo);

    rio::MemUtil::free(pOfficial);
    return result;
}

}
