#include <nn/ffl/FFLiCreateID.h>

static inline
FFLiCreateID* GetCreateID(FFLCreateID* pCreateID)
{
    return reinterpret_cast<FFLiCreateID*>(pCreateID);
}

static inline
const FFLiCreateID* GetCreateID(const FFLCreateID* pCreateID)
{
    return reinterpret_cast<const FFLiCreateID*>(pCreateID);
}

BOOL FFLiIsNullMiiID(const FFLCreateID* pCreateID)
{
    if (pCreateID != NULL)
    {
        const u32 count = FFL_CREATE_ID_SIZE / sizeof(u16);
        NN_STATIC_ASSERT(count == 5);

        if (pCreateID->value16[0] == 0 &&
            pCreateID->value16[1] == 0 &&
            pCreateID->value16[2] == 0 &&
            pCreateID->value16[3] == 0 &&
            pCreateID->value16[4] == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

void FFLiGetTemporaryMiiID(FFLCreateID* pCreateID)
{
    static const FFLiCreateID temporaryMiiID = {
        FFLI_CREATE_ID_FLAG_TEMPORARY
    };

    *GetCreateID(pCreateID) = temporaryMiiID;
}

BOOL FFLiIsNormalMiiID(const FFLCreateID* pCreateID)
{
    u8 flags = GetCreateID(pCreateID)->flags;
    return (flags & FFLI_CREATE_ID_FLAG_NORMAL) != 0;
}

BOOL FFLiIsSpecialMiiID(const FFLCreateID* pCreateID)
{
    return !FFLiIsNormalMiiID(pCreateID);
}

BOOL FFLiIsNTRMiiID(const FFLCreateID* pCreateID)
{
    u8 flags = GetCreateID(pCreateID)->flags;
    
    if (flags & FFLI_CREATE_ID_FLAG_UNKNOWN_0)
        return false;
    
    if (!(flags & FFLI_CREATE_ID_FLAG_UNKNOWN_2))
        return false;
    
    return true;
}

BOOL FFLiIsTemporaryMiiID(const FFLCreateID* pCreateID)
{
    u8 flags = GetCreateID(pCreateID)->flags;
    return (flags & FFLI_CREATE_ID_FLAG_TEMPORARY) != 0;
}

BOOL FFLiIsValidMiiID(const FFLCreateID* pCreateID)
{
    return !FFLiIsNullMiiID(pCreateID) && !FFLiIsTemporaryMiiID(pCreateID);
}

BOOL FFLiIsSameMiiID(const FFLCreateID* a, const FFLCreateID* b)
{
    if (FFLiIsValidMiiID(a) && FFLiIsValidMiiID(b))
    {
        const u32 count = FFL_CREATE_ID_SIZE / sizeof(u16);
        NN_STATIC_ASSERT(count == 5);

        if (a->value16[0] == b->value16[0] &&
            a->value16[1] == b->value16[1] &&
            a->value16[2] == b->value16[2] &&
            a->value16[3] == b->value16[3] &&
            a->value16[4] == b->value16[4])
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
