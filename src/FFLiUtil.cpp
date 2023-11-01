#include <nn/ffl/FFLiUtil.h>

u32 FFLiRoundUp(u32 value, u32 alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}

void* FFLiRoundUpPtr(void* ptr, u32 alignment)
{
    return (void*)FFLiRoundUp((u32)ptr, alignment);
}

bool FFLiCheckAlign(u32 value, u32 alignment)
{
    return (value & (alignment - 1)) == 0;
}

bool FFLiCheckAlignPtr(const void* ptr, u32 alignment)
{
    return FFLiCheckAlign((u32)ptr, alignment);
}

void FFLiCopyWcharT2U16(u16* dst, u32 size, const wchar_t* src)
{
    for (u32 i = 0, j = 0; i < size; i++)
        if ((dst[i] = src[j]) != L'\0')
            j++;
}

s32 FFLiCompareString16(const u16* s1, const u16* s2, s32 n)
{
    for (s32 i = 0; i < n; i++)
    {
        s32 cmp = s1[i] - s2[i];
        if (cmp != 0)
            return cmp;

        if (s1[i] == L'\0')
            break;
    }
    return 0;
}

bool FFLiIsNumberCharCode(u16 c)
{
    return (
        (L'0'   <= c && c <= L'9'  ) ||
        (0xFF10 <= c && c <= 0xFF19) || // 0xFF10 == L'０', 0xFF19 == L'９'
        (0x2160 <= c && c <= 0x2169) || // 0x2160 == L'Ⅰ', 0x2169 == L'Ⅹ'
        (0x2170 <= c && c <= 0x2179) || // 0x2170 == L'ⅰ', 0x2179 == L'ⅹ'
        (0x2460 <= c && c <= 0x2473) || // 0x2460 == L'①', 0x2473 == L'⑳'
        (0x2776 <= c && c <= 0x277E)    // 0x2776 == L'❶', 0x277E == L'❾'
    );
}
