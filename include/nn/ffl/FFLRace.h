#ifndef FFL_RACE_H_
#define FFL_RACE_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLRace
{
    FFL_RACE_0      = 0,
    FFL_RACE_1      = 1,
    FFL_RACE_2      = 2,
    FFL_RACE_MAX    = 3
}
FFLRace;

#ifdef __cplusplus
}
#endif

#endif // FFL_RACE_H_