#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiDatabaseFile.h>
#include <nn/ffl/FFLiDatabaseFileAccessor.h>
#include <nn/ffl/FFLiPath.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiFileWriteBuffer.h>

#include <filedevice/rio_FileDeviceMgr.h>

#include <cstring>

enum FFLiFsFileResult
{
    FFLI_FS_FILE_RESULT_OK                  = 0,
    FFLI_FS_FILE_RESULT_READ_BUFFER_EMPTY   = 1,
    FFLI_FS_FILE_RESULT_WRITE_BUFFER_EMPTY  = 2,
    FFLI_FS_FILE_RESULT_OUT_OF_MEMORY       = 3,
    FFLI_FS_FILE_RESULT_NOT_FOUND           = 4
};

struct FFLiFsResult
{
    FFLiFsFileResult    fileResult;
    rio::RawErrorCode   fsStatus;
};
NN_STATIC_ASSERT(sizeof(FFLiFsResult) == 8);

static bool CheckFFLiFsResult(const FFLiFsResult& result);
static FFLResult ConvertFFLiFsResultToFFLResult(const FFLiFsResult& fsResult, FFLResult result);

namespace {

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

FFLiFsResult LoadDatabaseHidden(FFLiDatabaseFileHidden* pHidden, const char* pPath);
FFLiFsResult SaveDatabaseHidden(const FFLiDatabaseFileHidden& hidden, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath);

FFLiFsResult LoadDatabaseOfficial(FFLiDatabaseFileOfficial* pOfficial, const char* pPath);
FFLiFsResult SaveDatabaseOfficial(const FFLiDatabaseFileOfficial& official, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath);

FFLiFsResult CopyDatabaseOfficial(const char* pPathTo, const char* pPathFrom, FFLiFileWriteBuffer* pWriteBuffer);

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
    std::memset(m_PathOfficial, 0, FFL_PATH_MAX_LEN);
    std::memset(m_PathBackup,   0, FFL_PATH_MAX_LEN);
    std::memset(m_PathHidden,   0, FFL_PATH_MAX_LEN);
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

    FFLiFsResult result = LoadDatabaseHidden(&m_pDatabaseFile->hidden, GetPathHidden());
    if (CheckFFLiFsResult(result))
    {
        if (m_pDatabaseFile->hidden.IsValid())
            needInitHidden = false;
    }
    else if (result.fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
    {
        return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);
    }

    if (needInitHidden)
    {
        m_pDatabaseFile->hidden.Init();
        m_IsFlushQuotaNeeded = true;

        result = SaveDatabaseHidden(m_pDatabaseFile->hidden, m_pFileWriteBuffer, GetPathHidden());
        if (!CheckFFLiFsResult(result))
            return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);
    }

    result = LoadDatabaseOfficial(&m_pDatabaseFile->official, GetPathOfficial());
    if (CheckFFLiFsResult(result))
    {
        if (m_pDatabaseFile->official.IsValid())
        {
            if (!IsExistFile(GetPathBackup()))
            {
                m_IsFlushQuotaNeeded = true;

                result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathBackup());
                if (!CheckFFLiFsResult(result))
                    return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);
            }

            return FFL_RESULT_OK;
        }
    }
    else if (result.fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
    {
        return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);
    }

    result = LoadDatabaseOfficial(&m_pDatabaseFile->official, GetPathBackup());
    if (CheckFFLiFsResult(result))
    {
        if (m_pDatabaseFile->official.IsValid())
        {
            m_IsFlushQuotaNeeded = true;

            result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathOfficial());
            if (!CheckFFLiFsResult(result))
                return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);

            return FFL_RESULT_OK;
        }
    }
    else if (result.fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
    {
        return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);
    }

    Init();

    m_IsFlushQuotaNeeded = true;

    result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathBackup());
    if (!CheckFFLiFsResult(result))
        return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);

    result = SaveDatabaseOfficial(m_pDatabaseFile->official, m_pFileWriteBuffer, GetPathOfficial());
    if (!CheckFFLiFsResult(result))
        return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_LOAD_ERROR);

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

    FFLiFsResult result = SaveDatabaseHidden(m_pDatabaseFile->hidden, m_pFileWriteBuffer, GetPathHidden());
    if (!CheckFFLiFsResult(result))
        return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_SAVE_ERROR);

    m_IsSaveHiddenNeeded = false;
    m_IsHiddenSaved = true;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseFileAccessor::BackupOfficial()
{
    const char* pPathFrom   = GetPathOfficial();
    const char* pPathTo     = GetPathBackup();

    FFLiFsResult result = CopyDatabaseOfficial(pPathTo, pPathFrom, m_pFileWriteBuffer);
    if (!CheckFFLiFsResult(result))
        return ConvertFFLiFsResultToFFLResult(result, FFL_RESULT_FILE_SAVE_ERROR);

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

static bool CheckFFLiFsResult(const FFLiFsResult& result)
{
    if (result.fileResult != FFLI_FS_FILE_RESULT_OK)
        return false;

    if (result.fsStatus != rio::RAW_ERROR_OK)
        return false;

    return true;
}

static FFLResult ConvertFFLiFsResultToFFLResult(const FFLiFsResult& fsResult, FFLResult result)
{
    if (fsResult.fileResult == FFLI_FS_FILE_RESULT_NOT_FOUND)
        return FFL_RESULT_FS_NOT_FOUND;

    if (fsResult.fileResult == FFLI_FS_FILE_RESULT_OUT_OF_MEMORY)
        return FFL_RESULT_OUT_OF_MEMORY;

    return result;
}

static bool CheckFileNotFound(rio::RawErrorCode status)
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

FFLiFsResult ReadFile(void* pDst, u32 size, const char* pPath)
{
    rio::NativeFileDevice* device = rio::FileDeviceMgr::instance()->getNativeFileDevice();

    rio::FileHandle fileHandle;
    if (!device->tryOpen(&fileHandle, pPath, rio::FileDevice::FILE_OPEN_FLAG_READ))
    {
        rio::RawErrorCode status = device->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, status };
    }

    u32 readSize = 0;
    if (!fileHandle.tryRead(&readSize, static_cast<u8*>(pDst), size))
    {
        fileHandle.tryClose();

        rio::RawErrorCode status = device->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, status };
    }

    if (readSize == 0)
    {
        fileHandle.tryClose();

        return FFLiFsResult { FFLI_FS_FILE_RESULT_READ_BUFFER_EMPTY };
    }
    else
    {
        if (!fileHandle.tryClose())
        {
            rio::RawErrorCode status = device->getLastRawError();
            RIO_ASSERT(status != rio::RAW_ERROR_OK);
            return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, status };
        }
    }

    return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
}

rio::RawErrorCode WriteFileImpl(rio::FileHandle& fileHandle, const void* pSrc, u32 size, FFLiFileWriteBuffer* pWriteBuffer)
{
    rio::RawErrorCode ret = rio::RawErrorCode(0);

    const u32 count = (size + (FFLI_FILE_WRITE_BUFFER_SIZE - 1)) / FFLI_FILE_WRITE_BUFFER_SIZE;

    for (u32 i = 0; i < count; i++)
    {
        u32 writeSize = FFLiMin(size - i * FFLI_FILE_WRITE_BUFFER_SIZE, FFLI_FILE_WRITE_BUFFER_SIZE);
        std::memcpy(pWriteBuffer, ((const u8*)pSrc) + i * FFLI_FILE_WRITE_BUFFER_SIZE, writeSize);

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

FFLiFsResult WriteFile(const void* pSrc, u32 size, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath)
{
    rio::NativeFileDevice* device = rio::FileDeviceMgr::instance()->getNativeFileDevice();

    rio::FileHandle fileHandle;
    if (!device->tryOpen(&fileHandle, pPath, rio::FileDevice::FILE_OPEN_FLAG_WRITE))
    {
        rio::RawErrorCode status = device->getLastRawError();
        RIO_ASSERT(status != rio::RAW_ERROR_OK);
        if (CheckFileNotFound(status))
            return FFLiFsResult { FFLI_FS_FILE_RESULT_NOT_FOUND };
        else
            return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, status };
    }

    rio::RawErrorCode status = WriteFileImpl(fileHandle, pSrc, size, pWriteBuffer);
    if (status == 0)
    {
        fileHandle.tryClose();

        return FFLiFsResult { FFLI_FS_FILE_RESULT_WRITE_BUFFER_EMPTY };
    }
    else if (status < 0)
    {
        fileHandle.tryClose();

        return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, status };
    }
    else
    {
        if (!fileHandle.tryClose())
        {
            rio::RawErrorCode status = device->getLastRawError();
            RIO_ASSERT(status != rio::RAW_ERROR_OK);
            return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, status };
        }
    }

    return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
}

FFLiFsResult LoadDatabaseHidden(FFLiDatabaseFileHidden* pHidden, const char* pPath)
{
    FFLiFsResult result = ReadFile(pHidden, sizeof(FFLiDatabaseFileHidden), pPath);
    if (!CheckFFLiFsResult(result))
        return result;

    return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
}

FFLiFsResult SaveDatabaseHidden(const FFLiDatabaseFileHidden& hidden, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath)
{
    return WriteFile(&hidden, sizeof(FFLiDatabaseFileHidden), pWriteBuffer, pPath);
}

FFLiFsResult LoadDatabaseOfficial(FFLiDatabaseFileOfficial* pOfficial, const char* pPath)
{
    FFLiFsResult result = ReadFile(pOfficial, sizeof(FFLiDatabaseFileOfficial), pPath);
    if (!CheckFFLiFsResult(result))
        return result;

    return FFLiFsResult { FFLI_FS_FILE_RESULT_OK, rio::RAW_ERROR_OK };
}

FFLiFsResult SaveDatabaseOfficial(const FFLiDatabaseFileOfficial& official, FFLiFileWriteBuffer* pWriteBuffer, const char* pPath)
{
    return WriteFile(&official, sizeof(FFLiDatabaseFileOfficial), pWriteBuffer, pPath);
}

FFLiFsResult CopyDatabaseOfficial(const char* pPathTo, const char* pPathFrom, FFLiFileWriteBuffer* pWriteBuffer)
{
    FFLiDatabaseFileOfficial* pOfficial = static_cast<FFLiDatabaseFileOfficial*>(rio::MemUtil::alloc(sizeof(FFLiDatabaseFileOfficial), rio::FileDevice::cBufferMinAlignment));
    if (pOfficial == NULL)
        return FFLiFsResult { FFLI_FS_FILE_RESULT_OUT_OF_MEMORY };

    FFLiFsResult result = LoadDatabaseOfficial(pOfficial, pPathFrom);
    if (CheckFFLiFsResult(result))
        result = SaveDatabaseOfficial(*pOfficial, pWriteBuffer, pPathTo);

    rio::MemUtil::free(pOfficial);
    return result;
}

}
