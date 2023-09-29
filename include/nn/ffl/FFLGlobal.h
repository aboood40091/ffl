#ifndef FFL_GLOBAL_H_
#define FFL_GLOBAL_H_

#include <nn/ffl/FFLColor.h>
#include <nn/ffl/FFLResult.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLInitDesc FFLInitDesc;
typedef struct FFLResourceDesc FFLResourceDesc;

FFLResult FFLInitResEx(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc);
void FFLInitResGPUStep(void);
u32 FFLGetWorkSize(const FFLInitDesc* pInitDesc);
FFLResult FFLFlushQuota(void);
FFLResult FFLExit(void);
BOOL FFLIsAvailable(void);
FFLColor FFLGetFavoriteColor(s32 index);

#ifdef __cplusplus
}
#endif

#endif // FFL_GLOBAL_H_
