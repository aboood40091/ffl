#include <nn/ffl/FFLResourceDesc.h>

#include <nn/ffl/FFLiCompressor.h>
#include <nn/ffl/FFLiDatabaseFile.h>
#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiResourceHeader.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiFileWriteBuffer.h>

#include <new>

// --------------------------------------------------------------------------

#include <nn/ffl/version.h>
#include <nn/middleware.h>

#define MACRO_TO_STR_2(x)   #x
#define MACRO_TO_STR(x)     MACRO_TO_STR_2(x)

#define FFL_MIDDLEWARE_SYMBOL "FFL_" MACRO_TO_STR(FFL_VERSION_MAJOR) "_" MACRO_TO_STR(FFL_VERSION_MINOR) "_" MACRO_TO_STR(FFL_VERSION_MICRO)

// --------------------------------------------------------------------------

static void SetResultIfError(FFLResult* pDst, FFLResult src)
{
    if (*pDst == FFL_RESULT_OK || *pDst == FFL_RESULT_ODB_EMPTY)
        if (src != FFL_RESULT_OK)
            *pDst = src;
}

// --------------------------------------------------------------------------

namespace {

FFLiManager* g_FFLManager = NULL;

FFLResult InitResImpl(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc);

u32 TempMemorySize(bool);

FFLiFileWriteBuffer* CreateFileWriteBuffer(FFLiBufferAllocator* pAllocator);
FFLiCompressor* CreateCompressor(const void* pShaderData, FFLiBufferAllocator* pAllocator);

}

FFLResult FFLiInitResEx(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc)
{
    return InitResImpl(pBuffer, pInitDesc, pResDesc);
}

void FFLiInitResGPUStep()
{
    if (FFLiManager::IsConstruct())
        FFLiManager::GetInstance()->SetupGPU();
}

u32 FFLiGetWorkSize(const FFLInitDesc* pInitDesc)
{
    return FFLiManager::GetBufferSize(pInitDesc);
}

FFLResult FFLiFlushQuota(bool force)
{
    if (!FFLiManager::IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    return FFLiManager::GetInstance()->FlushQuota(force);
}

FFLResult FFLiExit()
{
    return FFLiManager::Destroy();
}

bool FFLiIsAvailable()
{
    return FFLiManager::IsConstruct();
}

FFLResult FFLiManager::Create(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc)
{
    if (IsConstruct())
        return FFL_RESULT_OK;

    if (pInitDesc == NULL)
        return FFL_RESULT_ERROR;

    FFLiBufferAllocator allocator;
    allocator.Init(pBuffer, GetBufferSize(pInitDesc));

    g_FFLManager = new (allocator.Allocate(sizeof(FFLiManager), 4)) FFLiManager(pInitDesc, &allocator);

    FFLResult result = g_FFLManager->AfterConstruct(pInitDesc, pResDesc);
    if (result != FFL_RESULT_OK && result != FFL_RESULT_ODB_EMPTY)
        Destroy();
    
    return result;
}

FFLResult FFLiManager::Destroy()
{
    if (!IsConstruct())
        return FFL_RESULT_MANAGER_NOT_CONSTRUCT;

    FFLiManager* pManager = GetInstance();

    FFLResult result = pManager->BeforeDestruct();

    pManager->~FFLiManager();
    g_FFLManager = NULL;

    return result;
}

bool FFLiManager::IsConstruct()
{
    return g_FFLManager != NULL;
}

FFLiManager* FFLiManager::GetInstance()
{
    return g_FFLManager;
}

u32 FFLiManager::GetBufferSize(const FFLInitDesc* pInitDesc)
{
    if (pInitDesc == NULL)
        return 0;

    u32 ret  = 11400; // ???
    ret     += sizeof(FFLiResourceMultiHeader);
    ret     += sizeof(FFLiDatabaseFile);
    ret     += sizeof(FFLiFileWriteBuffer);

    if (pInitDesc->pShaderData != NULL)
    {
        ret += sizeof(FFLiCompressor);
        ret += FFLiCompressor::GetBufferSize(pInitDesc->pShaderData);
    }

    ret     += FFLiCopySurface::GetBufferSize();
    ret     += TempMemorySize(pInitDesc->_10);

    return ret;
}

FFLiManager::FFLiManager(const FFLInitDesc* pInitDesc, FFLiBufferAllocator* pAllocator)
    : m_pResourceMultiHeader(static_cast<FFLiResourceMultiHeader*>(pAllocator->Allocate(sizeof(FFLiResourceMultiHeader), FS_IO_BUFFER_ALIGN)))
    , m_pDatabaseFile(static_cast<FFLiDatabaseFile*>(pAllocator->Allocate(sizeof(FFLiDatabaseFile), FS_IO_BUFFER_ALIGN)))
    , m_pFileWriteBuffer(CreateFileWriteBuffer(pAllocator))
    , m_ResourceManager(m_pResourceMultiHeader, &m_FsClient)
    , m_DatabaseManager(m_pDatabaseFile, m_pFileWriteBuffer, &m_SystemContext, &m_FsClient, &m_Allocator)
    , m_CharModelCreateParam(&m_DatabaseManager, &m_ResourceManager, &m_ShaderCallback, pInitDesc->pShaderData != NULL)
    , m_InitDesc(*pInitDesc)
    , m_pCompressor(CreateCompressor(pInitDesc->pShaderData, pAllocator))
    , m_CopySurface(pAllocator)
    , m_IsSetupGPU(false)
    , _29ad(0)
{
    u32 allocatorSize = pAllocator->GetRestSize();
    m_Allocator.Init(pAllocator->Allocate(allocatorSize, 1), allocatorSize);
}

FFLiManager::~FFLiManager()
{
}

FFLResult FFLiManager::AfterConstruct(const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc)
{
    if (!m_SystemContext.AfterConstruct())
        return FFL_RESULT_UNKNOWN_17;

    if (!m_FsClient.Init(pInitDesc->pChangeParams) != FS_STATUS_OK)
        return FFL_RESULT_FS_ERROR;

    if (m_pCompressor != NULL)
        if (!m_pCompressor->SetupCPU(pInitDesc->pShaderData))
            return FFL_RESULT_ERROR;

    FFLResult result = m_ResourceManager.AfterConstruct();
    if (result != FFL_RESULT_OK)
        return result;

    if (pResDesc == NULL)
    {
        FFLiFsCommandBuffer* pCommandBuffer = static_cast<FFLiFsCommandBuffer*>(m_Allocator.Allocate(FFLI_FS_COMMAND_SIZE));
        result = m_ResourceManager.LoadResourceHeader(pCommandBuffer);
        m_Allocator.Free(pCommandBuffer);
        if (result != FFL_RESULT_OK)
        {
            m_FsClient.Shutdown();
            return result;
        }
    }
    else
    {
        for (u32 i = 0; i < FFL_RESOURCE_TYPE_MAX; i++)
        {
            result = m_ResourceManager.AttachCache(pResDesc[i].pData, pResDesc[i].size, FFLResourceType(i));
            if (result != FFL_RESULT_OK)
            {
                m_FsClient.Shutdown();
                return result;
            }
        }
    }

    result = m_DatabaseManager.AfterConstruct();
    if (result != FFL_RESULT_OK)
        return result;

    return FFL_RESULT_OK;
}

FFLResult FFLiManager::BeforeDestruct()
{
    FFLResult result = FFL_RESULT_OK;

    SetResultIfError(&result, m_DatabaseManager.BeforeDestruct());

    if (m_FsClient.ShutdownIfValid() != FS_STATUS_OK)
        SetResultIfError(&result, FFL_RESULT_FS_ERROR);

    return result;
}

bool FFLiManager::CanInitCharModel() const
{
    return m_CopySurface.CanInitCharModel(m_IsSetupGPU, m_pCompressor != NULL);
}

void FFLiManager::SetupGPU()
{
    m_IsSetupGPU = true;

    m_CopySurface.SetupGPU();

    if (m_pCompressor != NULL)
        m_pCompressor->SetupGPU();
}

FFLResult FFLiManager::FlushQuota(bool force)
{
    return m_DatabaseManager.FlushQuota(force);
}

namespace {

FFLResult InitResImpl(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc)
{
    NN_DEFINE_MIDDLEWARE(moduleInfo, "Nintendo", FFL_MIDDLEWARE_SYMBOL);
    NN_USING_MIDDLEWARE(moduleInfo);

    if (FFLiManager::IsConstruct())
        return FFL_RESULT_OK;
    
    return FFLiManager::Create(pBuffer, pInitDesc, pResDesc);
}

u32 TempMemorySize(bool b)
{
    // ???
    return b ? 0x0A0000 : 0x520000;
}

FFLiFileWriteBuffer* CreateFileWriteBuffer(FFLiBufferAllocator* pAllocator)
{
    if (!pAllocator->CanAllocate(sizeof(FFLiFileWriteBuffer), FS_IO_BUFFER_ALIGN))
        return NULL;

    return static_cast<FFLiFileWriteBuffer*>(pAllocator->Allocate(sizeof(FFLiFileWriteBuffer), FS_IO_BUFFER_ALIGN));
}

FFLiCompressor* CreateCompressor(const void* pShaderData, FFLiBufferAllocator* pAllocator)
{
    if (pShaderData == NULL)
        return NULL;

    return new (pAllocator->Allocate(sizeof(FFLiCompressor), 4)) FFLiCompressor(pAllocator, pShaderData);
}

}
