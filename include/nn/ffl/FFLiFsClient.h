#ifndef FFLI_FS_CLIENT_H_
#define FFLI_FS_CLIENT_H_

#include <nn/ffl/types.h>

#include <cafe/fs.h>

class FFLiFsClient
{
public:
    FFLiFsClient();
    ~FFLiFsClient();

private:
    FSClient    m_FSClient;
    bool        m_IsValid;
};
NN_STATIC_ASSERT(sizeof(FFLiFsClient) == 0x1701);

#endif // FFLI_FS_CLIENT_H_
