#include <nn/ffl/FFLiFsClient.h>

FFLiFsClient::FFLiFsClient()
    : m_IsValid(false)

{
}

FFLiFsClient::~FFLiFsClient()
{
}

namespace {

void StateChangeCallback(FSClient* pClient, FSVolumeState state, void* pContext)
{
    OSReport("client = %p state = %u context = %p lastError = %d\n", pClient, u32(state), pContext, FSGetLastError(pClient));
}

FSStateChangeParams s_Param = {
    StateChangeCallback
};

}

FSStatus FFLiFsClient::Init(const FSStateChangeParams* pChangeParams, FSRetFlag errHandling)
{
    FSStatus status = FSAddClient(&m_FSClient, errHandling);
    if (status != FS_STATUS_OK)
        return status;

    if (pChangeParams == NULL)
        pChangeParams = &s_Param;

    SetStateChangeNotification(const_cast<FSStateChangeParams*>(pChangeParams));
    m_IsValid = true;

    return FS_STATUS_OK;
}

void FFLiFsClient::SetStateChangeNotification(FSStateChangeParams* pChangeParams)
{
    FSSetStateChangeNotification(&m_FSClient, pChangeParams);
}

bool FFLiFsClient::IsValid() const
{
    return m_IsValid;
}

FSStatus FFLiFsClient::Shutdown(FSRetFlag errHandling)
{
    FSStatus status = FSDelClient(&m_FSClient, errHandling);
    m_IsValid = false;
    return status;
}

FSStatus FFLiFsClient::ShutdownIfValid(FSRetFlag errHandling)
{
    FSStatus status = FS_STATUS_OK;
    if (IsValid())
        status = Shutdown(errHandling);
    return status;
}
