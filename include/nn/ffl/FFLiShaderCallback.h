#ifndef FFLI_SHADER_CALLBACK_H_
#define FFLI_SHADER_CALLBACK_H_

#include <nn/ffl/types.h>

#include <cafe/mat.h>

struct FFLDrawParam;
struct FFLShaderCallback;

class FFLiShaderCallback
{
public:
    FFLiShaderCallback();
    ~FFLiShaderCallback();

    const FFLShaderCallback* Get() const
    {
        return m_pShaderCallback;
    }

    void Set(const FFLShaderCallback* pCallback);

    void CallSetMatrix(const Mat44& mat) const;
    void CallDraw(const FFLDrawParam& drawParam) const;
    void CallSetContextState() const;

private:
    const FFLShaderCallback*    m_pShaderCallback;
};
NN_STATIC_ASSERT(sizeof(FFLiShaderCallback) == 4);

#endif // FFLI_SHADER_CALLBACK_H_
