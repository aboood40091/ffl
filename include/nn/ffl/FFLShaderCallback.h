#ifndef FFL_SHADER_CALLBACK_H_
#define FFL_SHADER_CALLBACK_H_

#include <nn/ffl/types.h>

#include <math/rio_MathTypes.h>

#if RIO_IS_CAFE
#include <cafe/gx2.h>
#endif // RIO_IS_CAFE

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLDrawParam FFLDrawParam;

typedef struct FFLShaderCallback
{
    void*               pObj;
#if RIO_IS_CAFE
    GX2ContextState*    pContextState;
#else
    void*               _4;
#endif // RIO_IS_CAFE

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
