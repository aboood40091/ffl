#include <nn/ffl/FFLShaderCallback.h>

#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTemp.h>

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

void FFLiShaderCallback::CallSetMatrix(const Mat44& mat) const
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
    if (IsExist())
        FFLiTempSetContextState(m_pShaderCallback->pContextState);
}