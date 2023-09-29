#include <nn/ffl/FFLDatabase.h>

#include <nn/ffl/FFLiManager.h>

FFLResult FFLiGetStoreData(FFLStoreData* pStoreData, FFLDataSource dataSource, u16 index)
{
    if (!FFLiManager::IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    return FFLiManager::GetInstance()->GetDatabaseManager().GetStoreData(static_cast<FFLiStoreDataCFL*>(pStoreData), dataSource, index);
}