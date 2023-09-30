#ifndef FFLI_MANAGER_H_
#define FFLI_MANAGER_H_

#include <nn/ffl/FFLColor.h>
#include <nn/ffl/FFLInitDesc.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiCharModelCreateParam.h>
#include <nn/ffl/FFLiDatabaseManager.h>
#include <nn/ffl/FFLiFsClient.h>
#include <nn/ffl/FFLiResourceManager.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiSystemContext.h>

#include <nn/ffl/detail/FFLiAllocator.h>
#include <nn/ffl/detail/FFLiCopySurface.h>

class   FFLiCompressor;
struct  FFLiDatabaseFile;
struct  FFLiFileWriteBuffer;
struct  FFLiResourceMultiHeader;

class FFLiManager
{
public:
    static bool IsConstruct();
    static FFLiManager* GetInstance();

public:
    FFLiDatabaseManager& GetDatabaseManager()
    {
        return m_DatabaseManager;
    }

    const FFLiShaderCallback& GetShaderCallback() const
    {
        return m_ShaderCallback;
    }

    FFLiCharModelCreateParam& GetCharModelCreateParam()
    {
        return m_CharModelCreateParam;
    }

    const FFLiCharModelCreateParam& GetCharModelCreateParam() const
    {
        return m_CharModelCreateParam;
    }

    const FFLInitDesc& GetInitDesc() const
    {
        return m_InitDesc;
    }

    bool CanInitCharModel() const;

private:
    FFLiAllocator               m_Allocator;
    FFLiSystemContext           m_SystemContext;
    FFLiResourceMultiHeader*    m_pResourceMultiHeader;
    FFLiDatabaseFile*           m_pDatabaseFile;
    FFLiFileWriteBuffer*        m_pFileWriteBuffer;
    FFLiFsClient                m_FsClient;
    FFLiResourceManager         m_ResourceManager;
    FFLiDatabaseManager         m_DatabaseManager;
    FFLiShaderCallback          m_ShaderCallback;
    FFLiCharModelCreateParam    m_CharModelCreateParam;
    FFLInitDesc                 m_InitDesc;
    FFLiCompressor*             m_pCompressor;
    FFLiCopySurface             m_CopySurface;
    bool                        m_IsSetupGPU;
    u8                          _29ad;
};
NN_STATIC_ASSERT(sizeof(FFLiManager) == 0x29B0);

// --------------------------------------------------------------------------

struct FFLResourceDesc;

FFLResult FFLiInitResEx(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc);
void FFLiInitResGPUStep();
u32 FFLiGetWorkSize(const FFLInitDesc* pInitDesc);
FFLResult FFLiFlushQuota(bool force);
FFLResult FFLiExit();
bool FFLiIsAvailable();
const FFLColor& FFLiGetFavoriteColor(s32 index);

#endif // FFLI_MANAGER_H_
