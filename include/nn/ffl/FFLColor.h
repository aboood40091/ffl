#ifndef FFL_COLOR_H_
#define FFL_COLOR_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLColor
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
}
FFLColor;
NN_STATIC_ASSERT(sizeof(FFLColor) == 0x10);

#ifdef __cplusplus
}
#endif

#endif // FFL_COLOR_H_
