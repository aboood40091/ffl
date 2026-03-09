#ifndef FFL_DRAW_PARAM_H_
#define FFL_DRAW_PARAM_H_

#include <nn/ffl/FFLModulateParam.h>

#include <gpu/rio_Drawer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLAttributeBufferType
{
    FFL_ATTRIBUTE_BUFFER_TYPE_POSITION  = 0,
    FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD  = 1,
    FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL    = 2,
    FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT   = 3,
    FFL_ATTRIBUTE_BUFFER_TYPE_COLOR     = 4,
    FFL_ATTRIBUTE_BUFFER_TYPE_MAX       = 5
}
FFLAttributeBufferType;
NN_STATIC_ASSERT(FFL_ATTRIBUTE_BUFFER_TYPE_MAX == _FFL_ATTRIBUTE_BUFFER_TYPE_MAX);

typedef struct FFLAttributeBuffer
{
    u32     size;
    u32     stride;
    void*   ptr;
}
FFLAttributeBuffer;
NN_STATIC_ASSERT(sizeof(FFLAttributeBuffer) == FFL_ATTRIBUTE_BUFFER_SIZE);
NN_STATIC_ASSERT(alignof(FFLAttributeBuffer) == FFL_ATTRIBUTE_BUFFER_ALIGN);

typedef struct FFLAttributeBufferParam
{
    FFLAttributeBuffer  attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_MAX];
}
FFLAttributeBufferParam;
NN_STATIC_ASSERT(sizeof(FFLAttributeBufferParam) == FFL_ATTRIBUTE_BUFFER_PARAM_SIZE);
NN_STATIC_ASSERT(alignof(FFLAttributeBufferParam) == FFL_ATTRIBUTE_BUFFER_PARAM_ALIGN);

typedef struct FFLPrimitiveParam
{
    rio::Drawer::PrimitiveMode  primitiveType;
    u32                         indexCount;
    u32                         _8; // Deleted
    void*                       pIndexBuffer;
}
FFLPrimitiveParam;
NN_STATIC_ASSERT(sizeof(FFLPrimitiveParam) == FFL_PRIMITIVE_PARAM_SIZE);
NN_STATIC_ASSERT(alignof(FFLPrimitiveParam) == FFL_PRIMITIVE_PARAM_ALIGN);

typedef enum FFLCullMode
{
    FFL_CULL_MODE_NONE  = 0,
    FFL_CULL_MODE_BACK  = 1,
    FFL_CULL_MODE_FRONT = 2,
    FFL_CULL_MODE_MAX   = 3
}
FFLCullMode;
NN_STATIC_ASSERT(sizeof(FFLCullMode) == 4);

typedef struct FFLDrawParam
{
    FFLAttributeBufferParam attributeBufferParam;
    FFLModulateParam        modulateParam;
    FFLCullMode             cullMode;
    FFLPrimitiveParam       primitiveParam;
}
FFLDrawParam;
NN_STATIC_ASSERT(sizeof(FFLDrawParam) == FFL_DRAW_PARAM_SIZE);
NN_STATIC_ASSERT(alignof(FFLDrawParam) == FFL_DRAW_PARAM_ALIGN);

#ifdef __cplusplus
}
#endif

#endif // FFL_DRAW_PARAM_H_
