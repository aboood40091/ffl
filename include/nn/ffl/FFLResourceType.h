#ifndef FFL_RESOURCE_TYPE_H_
#define FFL_RESOURCE_TYPE_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLResourceType
{
    FFL_RESOURCE_TYPE_MIDDLE    = 0,
    FFL_RESOURCE_TYPE_HIGH      = 1,
    FFL_RESOURCE_TYPE_MAX       = 2
}
FFLResourceType;

#ifdef __cplusplus
}
#endif

#endif // FFL_RESOURCE_TYPE_H_
