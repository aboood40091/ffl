#ifndef FFL_CREATE_ID_H_
#define FFL_CREATE_ID_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FFL_CREATE_ID_SIZE  (10)

struct FFLCreateID
{
    union
    {
        u8 data[FFL_CREATE_ID_SIZE];
        u16 value16[FFL_CREATE_ID_SIZE / sizeof(u16)];
    };
};
NN_STATIC_ASSERT(sizeof(FFLCreateID) == FFL_CREATE_ID_SIZE);

#ifdef __cplusplus
}
#endif

#endif // FFL_CREATE_ID_H_
