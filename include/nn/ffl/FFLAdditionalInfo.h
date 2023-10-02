#ifndef FFL_ADDITIONAL_INFO_H_
#define FFL_ADDITIONAL_INFO_H_

#include <nn/ffl/FFLCreateID.h>
#include <nn/ffl/FFLColor.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLAdditionalInfo
{
    u16         name[10 + 1];
    u16         creatorName[10 + 1];
    FFLCreateID createID;
    FFLColor    facelineColor;
    union
    {
        struct
        {
            u32 gender          : 1;    // (MSB)
            u32 birthMonth      : 4;
            u32 birthDay        : 5;
            u32 favoriteColor   : 4;
            u32 height          : 7;
            u32 build           : 7;
            u32 ngWord          : 1;
            u32 fontRegion      : 2;
            u32 hairDir         : 1;    // (LSB)
        };

        u32 flags;
    };
    u8          faceType;
    u8          hairType;
}
FFLAdditionalInfo;
NN_STATIC_ASSERT(sizeof(FFLAdditionalInfo) == 0x50);

#ifdef __cplusplus
}
#endif

#endif // FFL_ADDITIONAL_INFO_H_
