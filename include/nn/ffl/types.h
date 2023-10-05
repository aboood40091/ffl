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

    FFL_HAIR_DIR_MAX = 2,
    FFL_MOUTH_COLOR_MAX = 5,
    FFL_GLASS_TYPE_MAX = 9,
    FFL_FAVORITE_COLOR_MAX = 12,

    FFLI_FACELINE_COLOR_NUM = 6,
    FFLI_HAIR_COLOR_NUM     = 8,
    FFLI_EYE_COLOR_R_NUM    = 3,
    FFLI_EYE_COLOR_B_NUM    = 6,
    FFLI_GLASS_COLOR_NUM    = 6,
    FFLI_MOUTH_COLOR_R_NUM  = FFL_MOUTH_COLOR_MAX,
    FFLI_MOUTH_COLOR_G_NUM  = FFL_MOUTH_COLOR_MAX,
    FFLI_FAVORITE_COLOR_NUM = FFL_FAVORITE_COLOR_MAX
};

#ifdef __cplusplus
}
#endif

#endif // FFL_TYPES_H_
