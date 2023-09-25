#ifndef FFLI_RESOURCE_MANAGER_H_
#define FFLI_RESOURCE_MANAGER_H_

#include <nn/ffl/detail/FFLiResourceCache.h>

struct  FFLiResourceMultiHeader;
class   FFLiFsClient;

class FFLiResourceManager
{
public:
    FFLiResourceManager();
    ~FFLiResourceManager();

private:
    FFLiResourceMultiHeader*    m_pResourceMultiHeader;
    FFLiFsClient*               m_pFsClient;
    FFLiResourceCache           m_ResourceCache;
    char                        m_Path[FFL_RESOURCE_TYPE_MAX][FFL_PATH_MAX_LEN];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceManager) == 0x218);

#endif // FFLI_RESOURCE_MANAGER_H_
