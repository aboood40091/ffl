#ifndef FFLI_SHADER_CALLBACK_H_
#define FFLI_SHADER_CALLBACK_H_

#include <nn/ffl/types.h>

#include <gfx/rio_Graphics.h>
#include <math/rio_MathTypes.h>

struct FFLDrawParam;
struct FFLShaderCallback;

class FFLiShaderCallback
{
public:
    FFLiShaderCallback();
    ~FFLiShaderCallback();

    bool IsExist() const;

    const FFLShaderCallback* Get() const
    {
        return m_pShaderCallback;
    }

    void Set(const FFLShaderCallback* pCallback);

    void CallApplyAlphaTestEnable() const
    {
        CallApplyAlphaTest(true, rio::Graphics::COMPARE_FUNC_GREATER, 0.0f);
    }

    void CallApplyAlphaTestDisable() const
    {
        CallApplyAlphaTest(false, rio::Graphics::COMPARE_FUNC_ALWAYS, 0.0f);
    }

    void CallApplyAlphaTest(bool enable, rio::Graphics::CompareFunc func, f32 ref) const;
    void CallSetMatrix(const rio::BaseMtx44f& mat) const;
    void CallDraw(const FFLDrawParam& drawParam) const;

private:
    const FFLShaderCallback*    m_pShaderCallback;
};
NN_STATIC_ASSERT(sizeof(FFLiShaderCallback) == 4);

#endif // FFLI_SHADER_CALLBACK_H_
