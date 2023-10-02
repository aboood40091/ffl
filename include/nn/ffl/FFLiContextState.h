#ifndef FFLI_CONTEXT_STATE_H_
#define FFLI_CONTEXT_STATE_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

class FFLiBufferAllocator;

class FFLiContextState
{
public:
    FFLiContextState();
    ~FFLiContextState();    // Deleted in NSMBU

    static u32 GetBufferSize();

    void Initialize(FFLiBufferAllocator* pAllocator);
    void Setup();
    void Bind(bool setup = true);

private:
    bool                m_IsSetup;
    GX2ContextState*    m_pGX2ContextState;

};
NN_STATIC_ASSERT(sizeof(FFLiContextState) == 8);

#endif // FFLI_CONTEXT_STATE_H_
