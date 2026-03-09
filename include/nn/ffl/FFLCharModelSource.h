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
NN_STATIC_ASSERT(sizeof(FFLCharModelSource) == NN_ALIGN_FOR_PTR(NN_ALIGN_FOR_PTR(4) + NN_PTR_SIZE + 2));

#ifdef __cplusplus
}
#endif

#endif // FFL_CHAR_MODEL_SOURCE_H_
