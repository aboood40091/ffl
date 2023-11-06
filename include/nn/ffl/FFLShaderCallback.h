#ifndef FFL_SHADER_CALLBACK_H_
#define FFL_SHADER_CALLBACK_H_

#include <nn/ffl/types.h>

#include <gfx/rio_Graphics.h>
#include <math/rio_MathTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLDrawParam FFLDrawParam;

typedef struct FFLShaderCallback
{
    void*               pObj;

    void (*pApplyAlphaTestFunc)(void* pObj, bool enable, rio::Graphics::CompareFunc func, f32 ref);
    void (*pDrawFunc)(void* pObj, const FFLDrawParam& drawParam);
    void (*pSetMatrixFunc)(void* pObj, const rio::BaseMtx44f& matrix);
}
FFLShaderCallback;
NN_STATIC_ASSERT(sizeof(FFLShaderCallback) == 0x10);

void FFLSetShaderCallback(const FFLShaderCallback* pCallback);

#ifdef __cplusplus
}
#endif

#endif // FFL_SHADER_CALLBACK_H_
