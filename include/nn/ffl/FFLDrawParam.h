#ifndef FFL_DRAW_PARAM_H_
#define FFL_DRAW_PARAM_H_

#include <nn/ffl/FFLModulateParam.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLDrawParam
{
    u32                 _0[60 / sizeof(u32)];
    FFLModulateParam    modulateParam;
    u32                 _54;
    u32                 _58[16 / sizeof(u32)];
}
FFLDrawParam;
NN_STATIC_ASSERT(sizeof(FFLDrawParam) == 0x68);

#ifdef __cplusplus
}
#endif

#endif // FFL_DRAW_PARAM_H_
