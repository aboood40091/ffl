#include <nn/ffl/FFLiOrderData.h>

void FFLiOrderData::Init()
{
    m_NextIndex = 0xffff;
    m_PrevIndex = 0xffff;
}

u16 FFLiOrderData::NextIndex() const
{
    return m_NextIndex;
}

u16 FFLiOrderData::PrevIndex() const
{
    return m_PrevIndex;
}
