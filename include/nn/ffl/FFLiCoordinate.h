#ifndef FFLI_COORDINATE_H_
#define FFLI_COORDINATE_H_

#include <nn/ffl/types.h>

struct FFLiSnorm10_10_10_2;
struct FFLiSnorm8_8_8_8;
struct FFLVec3;
struct FFLVec4;

class FFLiCoordinate
{
public:
    FFLiCoordinate();
    ~FFLiCoordinate();

    void Transform(FFLVec3* pVec) const;
    void Transform(FFLVec4* pVec) const;

    static f32 FlipValue(FFLVec3* pVec, f32 value);
    static f32 FlipValue(FFLVec4* pVec, f32 value);
    
    void TransformWithoutScale(FFLVec3* pVec) const;
    void TransformWithoutScale(FFLVec4* pVec) const;    // Deleted in NSMBU

    void TransformWithoutScale(FFLiSnorm10_10_10_2* pVec) const;
    void TransformWithoutScale(FFLiSnorm8_8_8_8* pVec) const;

    static u32 FlipValue(FFLiSnorm10_10_10_2* pVec, u32 value);
    static s8 FlipValue(FFLiSnorm8_8_8_8* pVec, s8 value);

    bool IsDefault() const;

private:
    template <typename T, typename ValueT>
    void TransformCommon(T* pVec) const
    {
        ValueT x = pVec->x;
        ValueT y = pVec->y;
        ValueT z = pVec->z;

        ValueT values[3] = { x, y, z };

        if (m_FlipX)
            x = FlipValue(pVec, x);

        if (m_FlipY)
            y = FlipValue(pVec, y);

        if (m_FlipZ)
            z = FlipValue(pVec, z);

        values[m_SwizzleX] = x;
        values[m_SwizzleY] = y;
        values[m_SwizzleZ] = z;

        pVec->x = values[0];
        pVec->y = values[1];
        pVec->z = values[2];
    }

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
