#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiSystem.h>

const FFLiAuthorID* FFLiGetUserID()
{
    if (!FFLiManager::IsConstruct())
        return NULL;

    return FFLiManager::GetInstance()->GetSystemContext().UserID();
}
