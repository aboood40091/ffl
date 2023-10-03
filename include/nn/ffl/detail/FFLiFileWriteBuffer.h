#ifndef FFLI_FILE_WRITE_BUFFER_H_
#define FFLI_FILE_WRITE_BUFFER_H_

#include <nn/ffl/types.h>

#define FFLI_FILE_WRITE_BUFFER_SIZE (0x40000u)

struct FFLiFileWriteBuffer
{
    u8  data[FFLI_FILE_WRITE_BUFFER_SIZE];
};
NN_STATIC_ASSERT(sizeof(FFLiFileWriteBuffer) == FFLI_FILE_WRITE_BUFFER_SIZE);

#endif // FFLI_FILE_WRITE_BUFFER_H_
