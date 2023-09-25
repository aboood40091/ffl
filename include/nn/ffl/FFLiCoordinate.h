#ifndef FFLI_COORDINATE_H_
#define FFLI_COORDINATE_H_

#include <nn/ffl/types.h>

class FFLiCoordinate
{
public:
    FFLiCoordinate();
    ~FFLiCoordinate();

private:
    u8      m_SwizzleY;
    u8      m_SwizzleZ;
    u8      m_SwizzleX;
    bool    m_FlipY;
    bool    m_FlipZ;
    bool    m_FlipX;
    f32     m_Scale;
};
NN_STATIC_ASSERT(sizeof(FFLiCoordinate) == 0xC);

#endif // FFLI_COORDINATE_H_
