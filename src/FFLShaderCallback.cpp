#include <nn/ffl/FFLShaderCallback.h>

#include <nn/ffl/FFLiManager.h>

void FFLSetShaderCallback(const FFLShaderCallback* pCallback)
{
    if (FFLiManager::IsConstruct())
        FFLiManager::GetInstance()->GetShaderCallback().Set(pCallback);
}
