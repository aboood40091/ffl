#ifndef FFL_TYPES_H_
#define FFL_TYPES_H_

#include <nn/types.h>
#include <nn/util/util_StaticAssert.h>

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    FFL_PATH_MAX_LEN = 256,

    FFLI_FACELINE_COLOR_NUM = 6,
    FFLI_HAIR_COLOR_NUM     = 8,
    FFLI_EYE_COLOR_R_NUM    = 3,
    FFLI_EYE_COLOR_B_NUM    = 6,
    FFLI_GLASS_COLOR_NUM    = 6,
    FFLI_MOUTH_COLOR_R_NUM  = 5,
    FFLI_MOUTH_COLOR_G_NUM  = 5,
    FFLI_FAVORITE_COLOR_NUM = 12
};

#ifdef __cplusplus
}
#endif

#endif // FFL_TYPES_H_