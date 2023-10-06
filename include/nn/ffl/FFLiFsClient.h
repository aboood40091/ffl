#ifndef FFLI_FS_CLIENT_H_
#define FFLI_FS_CLIENT_H_

#include <nn/ffl/types.h>

#include <cafe/fs.h>

class FFLiFsClient
{
public:
    FFLiFsClient();
    ~FFLiFsClient();

    FSStatus Init(const FSStateChangeParams* pChangeParams, FSRetFlag errHandling = FS_RET_NO_ERROR);
    void SetStateChangeNotification(FSStateChangeParams* pChangeParams);

    bool IsValid() const;

    FSStatus Shutdown(FSRetFlag errHandling = FS_RET_NO_ERROR);
    FSStatus ShutdownIfValid(FSRetFlag errHandling = FS_RET_NO_ERROR);

    FSClient& GetFSClient()
    {
        return m_FSClient;
    }

    const FSClient& GetFSClient() const
    {
        return m_FSClient;
    }

private:
    FSClient    m_FSClient;
    bool        m_IsValid;
};
NN_STATIC_ASSERT(sizeof(FFLiFsClient) == 0x1701);

#endif // FFLI_FS_CLIENT_H_
