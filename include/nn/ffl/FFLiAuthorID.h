#ifndef FFLI_AUTHOR_ID_H_
#define FFLI_AUTHOR_ID_H_

#include <nn/ffl/types.h>

struct FFLiAuthorID
{
    u16 _0;
    u16 _2;
    u16 _4;
    u16 _6;
};
NN_STATIC_ASSERT(sizeof(FFLiAuthorID) == 8);

#endif // FFLI_AUTHOR_ID_H_
