#ifndef FFL_MODULATE_PARAM_H_
#define FFL_MODULATE_PARAM_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLColor FFLColor;

typedef struct FFLModulateParam
{
    u32         _0;
    u32         _4;
    FFLColor*   pColorR;
    FFLColor*   pColorG;
    FFLColor*   pColorB;
    GX2Texture* pGX2Texture;
}
FFLModulateParam;
NN_STATIC_ASSERT(sizeof(FFLModulateParam) == 0x18);

#ifdef __cplusplus
}
#endif

#endif // FFL_MODULATE_PARAM_H_
