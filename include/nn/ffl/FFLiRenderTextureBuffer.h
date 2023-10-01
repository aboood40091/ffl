#ifndef FFLI_RENDER_TEXTURE_BUFFER_H_
#define FFLI_RENDER_TEXTURE_BUFFER_H_

#include <nn/ffl/types.h>

struct FFLiRenderTextureBuffer
{
    void*   pBuffer;
    u32     size;
};
NN_STATIC_ASSERT(sizeof(FFLiRenderTextureBuffer) == 8);

#endif // FFLI_RENDER_TEXTURE_BUFFER_H_
