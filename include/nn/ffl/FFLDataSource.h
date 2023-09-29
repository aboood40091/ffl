#ifndef FFL_DATA_SOURCE_H_
#define FFL_DATA_SOURCE_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLDataSource
{
    FFL_DATA_SOURCE_OFFICIAL            = 0,
    FFL_DATA_SOURCE_DEFAULT             = 1,
    FFL_DATA_SOURCE_MIDDLE_DB           = 2,
    FFL_DATA_SOURCE_STORE_DATA_OFFICIAL = 3,
    FFL_DATA_SOURCE_STORE_DATA          = 4,
    FFL_DATA_SOURCE_BUFFER              = 5
}
FFLDataSource;

#ifdef __cplusplus
}
#endif

#endif // FFL_DATA_SOURCE_H_
