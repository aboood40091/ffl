#ifndef FFL_DATABASE_H_
#define FFL_DATABASE_H_

#include <nn/ffl/FFLDataSource.h>
#include <nn/ffl/FFLStandard.h>
#include <nn/ffl/FFLResult.h>

#ifdef __cplusplus
extern "C" {
#endif

FFLResult FFLiGetStoreData(FFLStoreData* pStoreData, FFLDataSource dataSource, u16 index);

#ifdef __cplusplus
}
#endif

#endif // FFL_DATABASE_H_
