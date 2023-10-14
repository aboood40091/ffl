#include <nn/ffl/FFLiAuthorID.h>
#include <nn/ffl/FFLiSystem.h>

BOOL FFLiIsSameAuthorID(const FFLiAuthorID* a, const FFLiAuthorID* b)
{
    const u32 count = FFLI_AUTHOR_ID_SIZE / sizeof(u16);
    NN_STATIC_ASSERT(count == 4);

    if (a->value16[0] != b->value16[0])
        return false;

    if (a->value16[1] != b->value16[1])
        return false;

    if (a->value16[2] != b->value16[2])
        return false;

    if (a->value16[3] != b->value16[3])
        return false;

    return true;
}

BOOL FFLiIsHomeAuthorID(const FFLiAuthorID* pAuthorID)
{
    return FFLiIsSameAuthorID(pAuthorID, FFLiGetUserID());
}
