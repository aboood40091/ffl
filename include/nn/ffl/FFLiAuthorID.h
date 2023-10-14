#ifndef FFLI_AUTHOR_ID_H_
#define FFLI_AUTHOR_ID_H_

#include <nn/ffl/types.h>

#define FFLI_AUTHOR_ID_SIZE  (sizeof(u64))

typedef struct FFLiAuthorID
{
    union
    {
        u8 data[FFLI_AUTHOR_ID_SIZE];
        u16 value16[FFLI_AUTHOR_ID_SIZE / sizeof(u16)];
    };
}
FFLiAuthorID;
NN_STATIC_ASSERT(sizeof(FFLiAuthorID) == FFLI_AUTHOR_ID_SIZE);

BOOL FFLiIsSameAuthorID(const FFLiAuthorID* a, const FFLiAuthorID* b);
BOOL FFLiIsHomeAuthorID(const FFLiAuthorID* pAuthorID);

#endif // FFLI_AUTHOR_ID_H_
