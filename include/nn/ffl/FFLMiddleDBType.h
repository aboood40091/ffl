#ifndef FFL_MIDDLE_DB_TYPE_H_
#define FFL_MIDDLE_DB_TYPE_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLMiddleDBType
{
    FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_RANDOM_UPDATE       = 0,
    FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE_REVERSE = 1,
    FFL_MIDDLE_DB_TYPE_HIDDEN_PARAM_TIME_UPDATE         = 2,
    FFL_MIDDLE_DB_TYPE_RANDOM_PARAM                     = 3,
    FFL_MIDDLE_DB_TYPE_NET_PARAM                        = 4
}
FFLMiddleDBType;

#ifdef __cplusplus
}
#endif

#endif // FFL_MIDDLE_DB_TYPE_H_