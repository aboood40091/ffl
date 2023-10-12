#include <nn/ffl/FFLiTemp.h>

namespace {

void Invalidate();
bool ShouldCallInvalidate(GX2ShaderMode shaderMode);

}

void FFLiTempSetContextState(const GX2ContextState* pContextState)
{
    GX2SetContextState(pContextState);

    Invalidate();
}

void FFLiTempSetShaderMode(GX2ShaderMode shaderMode)
{
    GX2SetShaderMode(shaderMode);

    if (ShouldCallInvalidate(shaderMode))
        Invalidate();
}

namespace {

void Invalidate()
{
    GX2Invalidate(GX2_INVALIDATE_SHADER, NULL, 0xFFFFFFFF);
}

bool ShouldCallInvalidate(GX2ShaderMode shaderMode)
{
    return shaderMode != GX2_SHADER_MODE_UNIFORM_REGISTER;
}

}
