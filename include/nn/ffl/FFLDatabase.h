#ifndef FFL_DATABASE_H_
#define FFL_DATABASE_H_

#include <nn/ffl/FFLDataSource.h>
#include <nn/ffl/FFLResult.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLAdditionalInfo FFLAdditionalInfo;
typedef struct FFLStoreData FFLStoreData;

FFLResult FFLiGetStoreData(FFLStoreData* pStoreData, FFLDataSource dataSource, u16 index);

BOOL FFLIsAvailableOfficialData(u16 index);
BOOL FFLIsRegularOfficialData(u16 index);

FFLResult FFLGetAdditionalInfo(FFLAdditionalInfo* pAdditionalInfo, FFLDataSource dataSource, const void* pBuffer, u16 index, BOOL checkFontRegion);

void FFLiEnableSpecialMii(u32 key);

#ifdef __cplusplus
}
#endif

#endif // FFL_DATABASE_H_
