#include <nn/ffl/FFLiDateTime.h>
#include <nn/ffl/FFLiPath.h>
#include <nn/ffl/FFLiSystemContext.h>

#include <nn/act.h>

#include <cstring>

namespace nn { namespace act {

// Thanks for deprecating this, Nintendo... :/

Result GetTransferableIdEx(u64* pTransferableId, u32 uniqueId, u8 slotNo);

} } // namespace nn::act


FFLiSystemContext::FFLiSystemContext()
{
    Init(FFLiGetSecondsFrom_2000_01_01());
}

FFLiSystemContext::~FFLiSystemContext()
{
}

void FFLiSystemContext::Init(u32 seed)
{
    std::memset(&m_AuthorID, 0, sizeof(FFLiAuthorID));

    m_RandomContext.Init(seed);

    static const FFLiCreateIDBase DEFAULT_CREATE_ID_BASE = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    std::memcpy(&m_CreateIDBase, &DEFAULT_CREATE_ID_BASE, sizeof(FFLiCreateIDBase));

    m_TitleID = 0;
}

const FFLiAuthorID* FFLiSystemContext::UserID() const
{
    return &m_AuthorID;
}

u64 FFLiSystemContext::TitleID() const
{
    return m_TitleID;
}

bool FFLiSystemContext::AfterConstruct()
{
    if (nn::act::Initialize().IsFailure())
        return false;

    u64 id = 0;
    nn::Result result = nn::act::GetTransferableIdEx(&id, 0x004A0, 0xFF);

    if (nn::act::Finalize().IsFailure())
        return false;

    if (result.IsFailure())
        return false;

    std::memcpy(&m_AuthorID, &id, sizeof(FFLiAuthorID));
    m_TitleID = FFLiGetMiiStudioTitleID();
    return true;
}
