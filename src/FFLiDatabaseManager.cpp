#include <nn/ffl/FFLiCreateID.h>
#include <nn/ffl/FFLiDatabaseFile.h>
#include <nn/ffl/FFLiDatabaseManager.h>
#include <nn/ffl/FFLiMiddleDB.h>
#include <nn/ffl/FFLiMiiData.h>
#include <nn/ffl/FFLiSystemContext.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

#include <cstring>

FFLiDatabaseManager::FFLiDatabaseManager(FFLiDatabaseFile* pFile, FFLiFileWriteBuffer* pWriteBuffer, FFLiSystemContext* pContext, FFLiAllocator* pAllocator)
    : m_pSystemContext(pContext)
    , m_DatabaseFileAccessor(pFile, pWriteBuffer, pAllocator)
    , m_DatabaseRandom(pContext->RandomContext())
    , m_pAllocator(pAllocator)
{
    m_DatabaseFileAccessor.Init();
}

FFLiDatabaseManager::~FFLiDatabaseManager()
{
}

FFLResult FFLiDatabaseManager::AfterConstruct()
{
    FFLResult result = m_DatabaseFileAccessor.AfterConstruct(m_pSystemContext->TitleID());
    if (result != FFL_RESULT_OK)
        return result;

    result = m_DatabaseFileAccessor.BootLoad();
    if (result != FFL_RESULT_OK)
        return result;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseManager::BeforeDestruct()
{
    FFLResult result = m_DatabaseFileAccessor.BeforeDestruct();
    if (result != FFL_RESULT_OK)
        return result;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseManager::FlushQuota(bool force)
{
    FFLResult result = m_DatabaseFileAccessor.BeforeFlushQuota();
    if (result != FFL_RESULT_OK)
        return result;

    result = m_DatabaseFileAccessor.FlushQuota(force);
    if (result != FFL_RESULT_OK)
        return result;

    return FFL_RESULT_OK;
}

void FFLiDatabaseManager::EnableSpecialMii(u32 key)
{
    m_IsEnabledSpecialMii = key == 333326543;
}

bool FFLiDatabaseManager::IsEnabledSpecialMii() const
{
    return m_IsEnabledSpecialMii;
}

FFLResult FFLiDatabaseManager::GetCharInfoFromOfficial(FFLiCharInfo* pCharInfo, u16 index)
{
    if (!m_DatabaseFileAccessor.GetDatabaseFile()->official.Get(pCharInfo, index, true, IsEnabledSpecialMii()))
        return FFL_RESULT_FILE_INVALID;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseManager::GetCharInfoFromDefault(FFLiCharInfo* pCharInfo, u16 index)
{
    if (!m_DatabaseDefault.Get(pCharInfo, index))
        return FFL_RESULT_FILE_INVALID;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseManager::GetCharInfoFromStoreData(FFLiCharInfo* pCharInfo, FFLDataSource dataSource, const FFLiStoreDataCFL* pStoreDataCFL, u16 index)
{
    FFLResult result = FFLiStoreDataCFLToCharInfo(pCharInfo, pStoreDataCFL[index]);
    if (result != FFL_RESULT_OK)
        return result;

    u16 miiDataIndex;
    if (dataSource == FFL_DATA_SOURCE_STORE_DATA_OFFICIAL && m_DatabaseFileAccessor.GetDatabaseFile()->official.Search(&miiDataIndex, pCharInfo->creatorID))
    {
        result = GetCharInfoFromOfficial(pCharInfo, miiDataIndex);
        if (result != FFL_RESULT_OK)
            return result;
    }

    return FFL_RESULT_OK;
}

namespace {

void ReplaceNameIfAccord(u16* s, u32 size)
{
    bool found = false;

    for (u32 i = 0; i < size; i++)
    {
        if (s[i] == L'@' || s[i] == 0xFF20)   // 0xFF20 == L'＠', but I can't get it working with GHS
        {
            found = true;
            break;
        }
    }

    if (found)
        std::memcpy(s, L"???", sizeof(L"???"));
}

void ReplaceNameAndCreatorIfAccord(FFLiCharInfo* pCharInfo)
{
    ReplaceNameIfAccord(pCharInfo->name,        10 + 1);
    ReplaceNameIfAccord(pCharInfo->creatorName, 10 + 1);
}

}

FFLResult FFLiDatabaseManager::PickupCharInfo(FFLiCharInfo* pCharInfo, FFLDataSource dataSource, const void* pBuffer, u16 index)
{
    FFLResult result = FFL_RESULT_ERROR;

    u32 magic = 0;
    if (pBuffer != NULL)
        std::memcpy(&magic, pBuffer, sizeof(u32));

    switch (dataSource)
    {
    case FFL_DATA_SOURCE_BUFFER:
        std::memcpy(pCharInfo, pBuffer, sizeof(FFLiCharInfo));
        result = FFL_RESULT_OK;
        break;
    case FFL_DATA_SOURCE_OFFICIAL:
        result = GetCharInfoFromOfficial(pCharInfo, index);
        break;
    case FFL_DATA_SOURCE_DEFAULT:
        result = GetCharInfoFromDefault(pCharInfo, index);
        break;
    case FFL_DATA_SOURCE_MIDDLE_DB:
        if (magic == 0x46464D41)    // FFMA
            result = static_cast<const FFLiMiddleDB*>(pBuffer)->GetCharInfo(pCharInfo, index);
        break;
    case FFL_DATA_SOURCE_STORE_DATA_OFFICIAL:
    case FFL_DATA_SOURCE_STORE_DATA:
        result = GetCharInfoFromStoreData(pCharInfo, dataSource, static_cast<const FFLiStoreDataCFL*>(pBuffer), index);
        break;
    }

    ReplaceNameAndCreatorIfAccord(pCharInfo);

    if (result != FFL_RESULT_OK)
        return result;

    if (FFLiIsNullMiiID(&pCharInfo->creatorID) || !FFLiiVerifyCharInfo(pCharInfo, true))
        result = FFL_RESULT_FILE_INVALID;

    return result;
}

FFLResult FFLiDatabaseManager::GetStoreData(FFLiStoreDataCFL* pStoreDataCFL, FFLDataSource dataSource, u16 index)
{
    if (dataSource != FFL_DATA_SOURCE_OFFICIAL && dataSource != FFL_DATA_SOURCE_DEFAULT)
        return FFL_RESULT_ERROR;

    FFLiCharInfo charInfo;

    FFLResult result = PickupCharInfo(&charInfo, dataSource, NULL, index);
    if (result != FFL_RESULT_OK)
        return result;

    result = FFLiCharInfoToStoreDataCFL(pStoreDataCFL, &charInfo);
    if (result != FFL_RESULT_OK)
        return result;

    return FFL_RESULT_OK;
}

FFLResult FFLiDatabaseManager::UpdateMiddleDB(FFLiMiddleDB* pMiddleDB)
{
    pMiddleDB->ClearData();

    switch (pMiddleDB->Type())
    {
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_RANDOM_UPDATE:
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE_REVERSE:
    case FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE:
        return m_DatabaseFileAccessor.GetDatabaseFile()->hidden.UpdateMiddleDB(pMiddleDB, m_pAllocator);
    case FFL_MIDDLE_DB_TYPE_RANDOM_PARAM:
        return m_DatabaseRandom.UpdateMiddleDB(pMiddleDB);
    default:
        return FFL_RESULT_ERROR;
    }
}
