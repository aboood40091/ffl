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

struct FFLResourceDesc;

class   FFLiBufferAllocator;
class   FFLiCompressor;
struct  FFLiDatabaseFile;
struct  FFLiFileWriteBuffer;
struct  FFLiResourceMultiHeader;

FFLResult FFLiInitResEx(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc);
void FFLiInitResGPUStep();

u32 FFLiGetWorkSize(const FFLInitDesc* pInitDesc);

extern "C" FFLResult FFLiFlushQuota(bool force);    // No idea why this in particular is extern "C"

FFLResult FFLiExit();

bool FFLiIsAvailable();

// --------------------------------------------------------------------------

class FFLiManager
{
public:
    static FFLResult Create(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc);
    static FFLResult Destroy();

    static bool IsConstruct();
    static FFLiManager* GetInstance();

    static u32 GetBufferSize(const FFLInitDesc* pInitDesc);

private:
    FFLiManager(const FFLInitDesc* pInitDesc, FFLiBufferAllocator* pAllocator);
    ~FFLiManager();

    FFLResult AfterConstruct(const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc);
    FFLResult BeforeDestruct();

public:
    const FFLiSystemContext& GetSystemContext() const
    {
        return m_SystemContext;
    }

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

    FFLiCompressor* GetCompressor() const
    {
        return m_pCompressor;
    }

    FFLiCopySurface& GetCopySurface()
    {
        return m_CopySurface;
    }

    const FFLiCopySurface& GetCopySurface() const
    {
        return m_CopySurface;
    }

    bool CanInitCharModel() const;

    void SetupGPU();

    FFLResult FlushQuota(bool force);

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

#endif // FFLI_MANAGER_H_
