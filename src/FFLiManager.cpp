#include <nn/ffl/FFLResourceDesc.h>

#include <nn/ffl/FFLiDatabaseFile.h>
#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiResourceHeader.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiFileWriteBuffer.h>

#include <filedevice/rio_FileDevice.h>

#include <new>

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
    ret     += FFLiCopySurface::GetBufferSize();
    ret     += TempMemorySize(pInitDesc->_10);

    return ret;
}

FFLiManager::FFLiManager(const FFLInitDesc* pInitDesc, FFLiBufferAllocator* pAllocator)
    : m_pResourceMultiHeader(static_cast<FFLiResourceMultiHeader*>(pAllocator->Allocate(sizeof(FFLiResourceMultiHeader), rio::FileDevice::cBufferMinAlignment)))
    , m_pDatabaseFile(static_cast<FFLiDatabaseFile*>(pAllocator->Allocate(sizeof(FFLiDatabaseFile), rio::FileDevice::cBufferMinAlignment)))
    , m_pFileWriteBuffer(CreateFileWriteBuffer(pAllocator))
    , m_ResourceManager(m_pResourceMultiHeader)
    , m_DatabaseManager(m_pDatabaseFile, m_pFileWriteBuffer, &m_SystemContext, &m_Allocator)
    , m_CharModelCreateParam(&m_DatabaseManager, &m_ResourceManager, &m_ShaderCallback)
    , m_InitDesc(*pInitDesc)
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

    FFLResult result = m_ResourceManager.AfterConstruct();
    if (result != FFL_RESULT_OK)
        return result;

    if (pResDesc == NULL)
    {
        result = m_ResourceManager.LoadResourceHeader();
        if (result != FFL_RESULT_OK)
            return result;
    }
    else
    {
        for (u32 i = 0; i < FFL_RESOURCE_TYPE_MAX; i++)
        {
            result = m_ResourceManager.AttachCache(pResDesc->pData[i], pResDesc->size[i], FFLResourceType(i));
            if (result != FFL_RESULT_OK)
                return result;
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
    return result;
}

bool FFLiManager::CanInitCharModel() const
{
    return m_CopySurface.CanInitCharModel(m_IsSetupGPU);
}

void FFLiManager::SetupGPU()
{
    m_IsSetupGPU = true;

    m_CopySurface.SetupGPU();
}

FFLResult FFLiManager::FlushQuota(bool force)
{
    return m_DatabaseManager.FlushQuota(force);
}

namespace {

FFLResult InitResImpl(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc)
{
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
    if (!pAllocator->CanAllocate(sizeof(FFLiFileWriteBuffer), rio::FileDevice::cBufferMinAlignment))
        return NULL;

    return static_cast<FFLiFileWriteBuffer*>(pAllocator->Allocate(sizeof(FFLiFileWriteBuffer), rio::FileDevice::cBufferMinAlignment));
}

}
