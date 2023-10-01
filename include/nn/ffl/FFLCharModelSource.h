#ifndef FFL_CHAR_MODEL_SOURCE_H_
#define FFL_CHAR_MODEL_SOURCE_H_

#include <nn/ffl/FFLDataSource.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLCharModelSource
{
    FFLDataSource   dataSource;
    const void*     pBuffer;
    u16             index;
}
FFLCharModelSource;
NN_STATIC_ASSERT(sizeof(FFLCharModelSource) == 0xC);

#ifdef __cplusplus
}
#endif

#endif // FFL_CHAR_MODEL_SOURCE_H_
