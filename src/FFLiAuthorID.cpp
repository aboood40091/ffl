#include <nn/ffl/FFLiAuthorID.h>
#include <nn/ffl/FFLiSystem.h>

bool FFLiIsSameAuthorID(const FFLiAuthorID* a, const FFLiAuthorID* b)
{
    // TODO: This function is implemented without a loop somehow

    const u32 count = FFLI_AUTHOR_ID_SIZE / sizeof(u16);
    for (u32 i = 0; i < count; i++)
        if (a->value16[i] != b->value16[i])
            return false;

    return true;
}

bool FFLiIsHomeAuthorID(const FFLiAuthorID* pAuthorID)
{
    return FFLiIsSameAuthorID(pAuthorID, FFLiGetUserID());
}
