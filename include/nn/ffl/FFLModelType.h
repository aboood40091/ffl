#ifndef FFL_MODEL_TYPE_H_
#define FFL_MODEL_TYPE_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLModelType
{
    FFL_MODEL_TYPE_0    = 0,
    FFL_MODEL_TYPE_1    = 1,
    FFL_MODEL_TYPE_2    = 2,
    FFL_MODEL_TYPE_MAX  = 3
}
FFLModelType;

#ifdef __cplusplus
}
#endif

#endif // FFL_MODEL_TYPE_H_
