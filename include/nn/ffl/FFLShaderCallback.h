#ifndef FFL_SHADER_CALLBACK_H_
#define FFL_SHADER_CALLBACK_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>
#include <cafe/mat.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FFLDrawParam FFLDrawParam;

typedef struct FFLShaderCallback
{
    void*                   pObj;
    const GX2ContextState*  pContextState;

    void (*pDrawFunc)(void* pObj, const FFLDrawParam& drawParam);
    void (*pSetMatrixFunc)(void* pObj, const Mat44& matrix);
}
FFLShaderCallback;
NN_STATIC_ASSERT(sizeof(FFLShaderCallback) == 0x10);

#ifdef __cplusplus
}
#endif

#endif // FFL_SHADER_CALLBACK_H_
