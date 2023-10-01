#ifndef FFLI_RESOURCE_LOADER_H_
#define FFLI_RESOURCE_LOADER_H_

#include <nn/ffl/FFLResourceType.h>

#include <nn/ffl/FFLiFsFile.h>

class FFLiFsCommand;
class FFLiResourceLoaderBuffer;
class FFLiResourceManager;

class FFLiResourceLoader
{
public:
    FFLiResourceLoader(FFLiResourceManager* pResourceManager, FFLiResourceLoaderBuffer* pResLoaderBuffer, FFLResourceType resourceType);
    ~FFLiResourceLoader();

private:
    FFLiResourceManager*        m_pResourceManager;
    FFLiResourceLoaderBuffer*   m_pBuffer;
    FFLiFsCommand*              m_pFsCommand;
    FFLiFsFile                  m_FsFile;
    FFLResourceType             m_ResourceType;
};
NN_STATIC_ASSERT(sizeof(FFLiResourceLoader) == 0x18);

#endif // FFLI_RESOURCE_LOADER_H_
