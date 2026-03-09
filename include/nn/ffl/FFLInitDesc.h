#ifndef FFL_INIT_DESC_H_
#define FFL_INIT_DESC_H_

#include <nn/ffl/FFLFontRegion.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLInitDesc
{
    const void*             _0; // Deleted
    void*                   _4; // Deleted
    FFLFontRegion           fontRegion;
    BOOL                    _c;
    BOOL                    _10;
}
FFLInitDesc;
NN_STATIC_ASSERT(sizeof(FFLInitDesc) == NN_ALIGN_FOR_PTR(2 * NN_PTR_SIZE + 4 + 4 + 4));

#ifdef __cplusplus
}
#endif

#endif // FFL_INIT_DESC_H_
