#include <nn/ffl/FFLDatabase.h>

#include <nn/ffl/FFLiAdditionalInfo.h>
#include <nn/ffl/FFLiDatabaseFile.h>
#include <nn/ffl/FFLiManager.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

FFLResult FFLiGetStoreData(FFLStoreData* pStoreData, FFLDataSource dataSource, u16 index)
{
    if (!FFLiManager::IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    return FFLiManager::GetInstance()->GetDatabaseManager().GetStoreData(static_cast<FFLiStoreDataCFL*>(pStoreData), dataSource, index);
}

BOOL FFLIsAvailableOfficialData(u16 index)
{
    if (FFLiManager::IsConstruct())
    {
        FFLiManager* pManager = FFLiManager::GetInstance();
        FFLiDatabaseManager& databaseManager = pManager->GetDatabaseManager();
        FFLiDatabaseFile* pDatabaseFile = databaseManager.GetDatabaseFileAccessor().GetDatabaseFile();
        return pDatabaseFile->official.IsAvailable(index, true, databaseManager.IsEnabledSpecialMii());
    }
    return false;
}

BOOL FFLIsRegularOfficialData(u16 index)
{
    if (FFLiManager::IsConstruct())
    {
        FFLiManager* pManager = FFLiManager::GetInstance();
        FFLiDatabaseManager& databaseManager = pManager->GetDatabaseManager();
        FFLiDatabaseFile* pDatabaseFile = databaseManager.GetDatabaseFileAccessor().GetDatabaseFile();
        return pDatabaseFile->official.IsRegular(index, true, databaseManager.IsEnabledSpecialMii());
    }
    return false;
}

FFLResult FFLGetAdditionalInfo(FFLAdditionalInfo* pAdditionalInfo, FFLDataSource dataSource, const void* pBuffer, u16 index, BOOL checkFontRegion)
{
    if (!FFLiManager::IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    FFLiManager* pManager = FFLiManager::GetInstance();
    FFLiCharInfo charInfo;
    FFLResult result = pManager->GetDatabaseManager().PickupCharInfo(&charInfo, dataSource, pBuffer, index);
    if (result == FFL_RESULT_OK)
        FFLiGetAdditionalInfo(pAdditionalInfo, &charInfo, checkFontRegion, pManager->GetInitDesc().fontRegion);
    return result;
}

void FFLiEnableSpecialMii(u32 key)
{
    if (!FFLiManager::IsConstruct())
        return;

    FFLiManager::GetInstance()->GetDatabaseManager().EnableSpecialMii(key);
}
