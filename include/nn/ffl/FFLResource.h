#ifndef FFL_RESOURCE_H_
#define FFL_RESOURCE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#ifdef __cplusplus
extern "C" {
#endif

FFLResult FFLGetResourcePath(char* pDst, u32 size, FFLResourceType resourceType, BOOL LG);

#ifdef __cplusplus
}
#endif

#endif // FFL_RESOURCE_H_