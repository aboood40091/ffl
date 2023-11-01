#include <nn/ffl/FFLShaderCallback.h>

#include <nn/ffl/FFLiShaderCallback.h>

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

void FFLiShaderCallback::CallSetContextState() const
{
#if RIO_IS_CAFE
    if (IsExist())
    {
        GX2SetContextState(m_pShaderCallback->pContextState);
        GX2Invalidate(GX2_INVALIDATE_SHADER, NULL, 0xFFFFFFFF);
    }
#endif // RIO_IS_CAFE
}
