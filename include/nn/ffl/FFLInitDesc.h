#ifndef FFL_INIT_DESC_H_
#define FFL_INIT_DESC_H_

#include <nn/ffl/FFLFontRegion.h>

#include <cafe/fs.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLInitDesc
{
    const void*             pShaderData;
    FSStateChangeParams*    pChangeParams;
    FFLFontRegion           fontRegion;
    BOOL                    _c;
    BOOL                    _10;
}
FFLInitDesc;
NN_STATIC_ASSERT(sizeof(FFLInitDesc) == 0x14);

#ifdef __cplusplus
}
#endif

#endif // FFL_INIT_DESC_H_
