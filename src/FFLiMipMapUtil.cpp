#include <nn/ffl/FFLiMipMapUtil.h>

s32 FFLiGetMipMapNum(s32 width, s32 height)
{
    s32 num = 0;

    while (width > 32 && height > 32)
    {
        num++;

        width /= 2;
        height /= 2;
    }

    return num + 1;
}

u32 FFLiGetMipMapLevelSize(u32 size, u32 mipLevel)
{
    if (mipLevel == 0)
        return size;

    return FFLiGetMipMapLevelSize(size / 2, mipLevel - 1);
}
