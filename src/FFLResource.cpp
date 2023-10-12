#include <nn/ffl/FFLResource.h>

#include <nn/ffl/FFLiResourceManager.h>

FFLResult FFLGetResourcePath(char* pDst, u32 size, FFLResourceType resourceType, BOOL LG)
{
    return FFLiResourceManager::GetResourcePath(pDst, size, resourceType, LG);
}
