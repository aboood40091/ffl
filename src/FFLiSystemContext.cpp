#include <nn/ffl/FFLiDateTime.h>
#include <nn/ffl/FFLiPath.h>
#include <nn/ffl/FFLiSystemContext.h>

#if RIO_IS_CAFE
#include <nn/act.h>
#endif // RIO_IS_CAFE

FFLiSystemContext::FFLiSystemContext()
{
    Init(FFLiGetSecondsFrom_2000_01_01());
}

FFLiSystemContext::~FFLiSystemContext()
{
}

void FFLiSystemContext::Init(u32 seed)
{
    rio::MemUtil::set(&m_AuthorID, 0, sizeof(FFLiAuthorID));

    m_RandomContext.Init(seed);

    static const FFLiCreateIDBase DEFAULT_CREATE_ID_BASE = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    rio::MemUtil::copy(&m_CreateIDBase, &DEFAULT_CREATE_ID_BASE, sizeof(FFLiCreateIDBase));

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
#if RIO_IS_CAFE
    if (nn::act::Initialize().IsFailure())
        return false;

    u64 id = 0;
    nn::Result result = nn::act::GetTransferableIdEx(&id, 0x004A0, 0xFF);

    if (nn::act::Finalize().IsFailure())
        return false;

    if (result.IsFailure())
        return false;

    rio::MemUtil::copy(&m_AuthorID, &id, sizeof(FFLiAuthorID));
#else
    rio::MemUtil::set(&m_AuthorID, 0, sizeof(FFLiAuthorID));
#endif // RIO_IS_CAFE
    m_TitleID = FFLiGetMiiStudioTitleID();
    return true;
}
