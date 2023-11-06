#include <nn/ffl/FFLShaderCallback.h>

#include <nn/ffl/FFLiShaderCallback.h>

#if RIO_IS_CAFE
#include <gx2/mem.h>
#endif

FFLiShaderCallback::FFLiShaderCallback()
    : m_pShaderCallback(NULL)
{
}

FFLiShaderCallback::~FFLiShaderCallback()
{
}

bool FFLiShaderCallback::IsExist() const
{
    return m_pShaderCallback != NULL;
}

void FFLiShaderCallback::Set(const FFLShaderCallback* pCallback)
{
    m_pShaderCallback = pCallback;
}

void FFLiShaderCallback::CallApplyAlphaTest(bool enable, rio::Graphics::CompareFunc func, f32 ref) const
{
    if (IsExist())
        (*m_pShaderCallback->pApplyAlphaTestFunc)(m_pShaderCallback->pObj, enable, func, ref);
}

void FFLiShaderCallback::CallSetMatrix(const rio::BaseMtx44f& mat) const
{
    if (IsExist())
        (*m_pShaderCallback->pSetMatrixFunc)(m_pShaderCallback->pObj, mat);
}

void FFLiShaderCallback::CallDraw(const FFLDrawParam& drawParam) const
{
    if (IsExist())
        (*m_pShaderCallback->pDrawFunc)(m_pShaderCallback->pObj, drawParam);
}
