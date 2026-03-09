#ifndef FFL_EXPRESSION_H_
#define FFL_EXPRESSION_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLExpression
{
    FFL_EXPRESSION_MAX = 19
}
FFLExpression;
NN_STATIC_ASSERT(sizeof(FFLExpression) == 4);
NN_STATIC_ASSERT(FFL_EXPRESSION_MAX == _FFL_EXPRESSION_MAX);

#ifdef __cplusplus
}
#endif

#endif // FFL_EXPRESSION_H_
