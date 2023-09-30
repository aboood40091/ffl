#ifndef FFL_RESOLUTION_H_
#define FFL_RESOLUTION_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLResolution
{
    FFL_RESOLUTION_MASK                 = 0x3fffffff,
    FFL_RESOLUTION_MIP_MAP_ENABLE_MASK  = 1 << 30
}
FFLResolution;

#ifdef __cplusplus
}
#endif

#endif // FFL_RESOLUTION_H_
