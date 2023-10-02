#include <nn/ffl/FFLiContextState.h>
#include <nn/ffl/FFLiTemp.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

FFLiContextState::FFLiContextState()
    : m_IsSetup(false)
    , m_pGX2ContextState(NULL)
{
}

FFLiContextState::~FFLiContextState()
{
}

u32 FFLiContextState::GetBufferSize()
{
    return GX2_CONTEXT_STATE_ALIGNMENT + sizeof(GX2ContextState);
}

void FFLiContextState::Initialize(FFLiBufferAllocator* pAllocator)
{
    m_pGX2ContextState = static_cast<GX2ContextState*>(pAllocator->Allocate(sizeof(GX2ContextState), GX2_CONTEXT_STATE_ALIGNMENT));
}

void FFLiContextState::Setup()
{
    m_IsSetup = true;
    GX2SetupContextState(m_pGX2ContextState);
}

void FFLiContextState::Bind(bool setup)
{
    if (setup && !m_IsSetup)
        Setup();

    else
        FFLiTempSetContextState(m_pGX2ContextState);
}
