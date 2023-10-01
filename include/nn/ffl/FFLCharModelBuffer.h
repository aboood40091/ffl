#ifndef FFL_CHAR_MODEL_BUFFER_H_
#define FFL_CHAR_MODEL_BUFFER_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLCharModelBuffer
{
    void*   pBuffer;
    u32     bufferSize;
    void*   pTempBuffer;
    u32     tempBufferSize;
}
FFLCharModelBuffer;
NN_STATIC_ASSERT(sizeof(FFLCharModelBuffer) == 0x10);

#ifdef __cplusplus
}
#endif

#endif // FFL_CHAR_MODEL_BUFFER_H_
