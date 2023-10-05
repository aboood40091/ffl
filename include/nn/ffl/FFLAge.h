#ifndef FFL_AGE_H_
#define FFL_AGE_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLAge
{
    FFL_AGE_0   = 0,
    FFL_AGE_1   = 1,
    FFL_AGE_2   = 2,
    FFL_AGE_MAX = 3
}
FFLAge;

#ifdef __cplusplus
}
#endif

#endif // FFL_AGE_H_