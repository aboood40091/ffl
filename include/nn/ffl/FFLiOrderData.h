#ifndef FFLI_ORDER_DATA_H_
#define FFLI_ORDER_DATA_H_

#include <nn/ffl/types.h>

class FFLiOrderData
{
private:
    u16                 m_NextIndex;
    u16                 m_PrevIndex;
};
NN_STATIC_ASSERT_IS_POD(FFLiOrderData);
NN_STATIC_ASSERT(sizeof(FFLiOrderData) == 4);

#endif // FFLI_ORDER_DATA_H_
