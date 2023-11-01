#include <nn/ffl/FFLVec.h>

#include <nn/ffl/FFLiCoordinate.h>

FFLiCoordinate::FFLiCoordinate()
    : m_SwizzleY(1)
    , m_SwizzleZ(2)
    , m_SwizzleX(0)
    , m_FlipY(false)
    , m_FlipZ(false)
    , m_FlipX(false)
    , m_Scale(1.0f)
{
}

FFLiCoordinate::~FFLiCoordinate()
{
}

void FFLiCoordinate::Transform(FFLVec3* pVec) const
{
    TransformCommon<FFLVec3, f32>(pVec);
    pVec->x *= m_Scale;
    pVec->y *= m_Scale;
    pVec->z *= m_Scale;
}

void FFLiCoordinate::Transform(FFLVec4* pVec) const
{
    Transform((FFLVec3*)pVec);
}

f32 FFLiCoordinate::FlipValue(FFLVec3*, f32 value)
{
    return -value;
};

f32 FFLiCoordinate::FlipValue(FFLVec4*, f32 value)
{
    return -value;
};

void FFLiCoordinate::TransformWithoutScale(FFLVec3* pVec) const
{
    TransformCommon<FFLVec3, f32>(pVec);
}

void FFLiCoordinate::TransformWithoutScale(FFLVec4* pVec) const
{
    TransformWithoutScale((FFLVec3*)pVec);
}

void FFLiCoordinate::TransformWithoutScale(FFLiSnorm10_10_10_2* pVec) const
{
    TransformCommon<FFLiSnorm10_10_10_2, u32>(pVec);
}

void FFLiCoordinate::TransformWithoutScale(FFLiSnorm8_8_8_8* pVec) const
{
    TransformCommon<FFLiSnorm8_8_8_8, s8>(pVec);
}

u32 FFLiCoordinate::FlipValue(FFLiSnorm10_10_10_2* pVec, u32 value)
{
    if (value == 0x200)
        return 0x1FF;

    else if (value == 0x1FF)
        return 0x200;

    else
        return (0x400 - value) & 0x3ff;

}

s8 FFLiCoordinate::FlipValue(FFLiSnorm8_8_8_8* pVec, s8 value)
{
    if (value == -128)
        value = 127;

    return -value;
}

bool FFLiCoordinate::IsDefault() const
{
    if (m_FlipY)
        return false;

    if (m_FlipZ)
        return false;

    if (m_FlipX)
        return false;

    if (m_SwizzleY != 1)
        return false;

    if (m_SwizzleZ != 2)
        return false;

    if (m_SwizzleX != 0)
        return false;

    if (m_Scale != 1.0f)
        return false;

    return true;
}
