#ifndef FFLI_CONTEXT_STATE_H_
#define FFLI_CONTEXT_STATE_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

class FFLiContextState
{
public:
    FFLiContextState();
    ~FFLiContextState();

private:
    bool                _0;
    GX2ContextState*    m_pGX2ContextState;

};
NN_STATIC_ASSERT(sizeof(FFLiContextState) == 8);

#endif // FFLI_CONTEXT_STATE_H_
