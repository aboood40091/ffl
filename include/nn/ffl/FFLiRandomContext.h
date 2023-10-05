#ifndef FFLI_RANDOM_CONTEXT_H_
#define FFLI_RANDOM_CONTEXT_H_

#include <nn/ffl/types.h>

class FFLiRandomContext
{
public:
    u16 Random(u16 max);

private:
    u32 m_RndX;
    u32 m_RndY;
    u32 m_RndZ;
};
NN_STATIC_ASSERT(sizeof(FFLiRandomContext) == 0xC);

#endif // FFLI_RANDOM_CONTEXT_H_
