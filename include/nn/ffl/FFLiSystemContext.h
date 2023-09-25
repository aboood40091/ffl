#ifndef FFLI_SYSTEM_CONTEXT_H_
#define FFLI_SYSTEM_CONTEXT_H_

#include <nn/ffl/FFLiAuthorID.h>
#include <nn/ffl/FFLiRandomContext.h>

class FFLiSystemContext
{
public:
    FFLiSystemContext();
    ~FFLiSystemContext();

private:
    FFLiRandomContext   m_RandomContext;
    FFLiAuthorID        m_AuthorID;
    u8                  _14[6];
    u64                 m_TitleID;
};
NN_STATIC_ASSERT(sizeof(FFLiSystemContext) == 0x28);

#endif // FFLI_SYSTEM_CONTEXT_H_
