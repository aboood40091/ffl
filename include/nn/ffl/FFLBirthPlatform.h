#ifndef FFL_BIRTH_PLATFORM_H_
#define FFL_BIRTH_PLATFORM_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLBirthPlatform
{
    FFL_BIRTH_PLATFORM_WII     = 1,
    FFL_BIRTH_PLATFORM_NTR     = 2,
    FFL_BIRTH_PLATFORM_CTR     = 3,
    FFL_BIRTH_PLATFORM_WII_U   = 4
}
FFLBirthPlatform;

#ifdef __cplusplus
}
#endif

#endif // FFL_BIRTH_PLATFORM_H_