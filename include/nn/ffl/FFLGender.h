#ifndef FFL_GENDER_H_
#define FFL_GENDER_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLGender
{
    FFL_GENDER_MALE     = 0,
    FFL_GENDER_FEMALE   = 1,
    FFL_GENDER_MAX      = 2
}
FFLGender;

#ifdef __cplusplus
}
#endif

#endif // FFL_GENDER_H_
