#ifndef FFLI_CHAR_MODEL_CREATOR_H_
#define FFLI_CHAR_MODEL_CREATOR_H_

#include <nn/ffl/FFLResult.h>

struct FFLCharModelSource;
struct FFLCharModelBuffer;
struct FFLCharModelDesc;
struct FFLShaderCallback;

class   FFLiBufferAllocator;
struct  FFLiCharModel;
class   FFLiCharModelCreateParam;
class   FFLiManager;
struct  FFLiRenderTextureBuffer;

class FFLiCharModelCreator
{
public:
    FFLiCharModelCreator(FFLiCharModelCreateParam* pParam, FFLiManager* pManager);
    ~FFLiCharModelCreator();

    FFLResult ExecuteCPUStep(FFLiCharModel* pModel, const FFLCharModelSource* pSource, const FFLCharModelDesc* pDesc, FFLCharModelBuffer* pBuffer);
    void ExecuteGPUStep(FFLiCharModel* pModel, const FFLShaderCallback* pCallback);
    void AfterExecuteGPUStep(FFLiCharModel* pModel);

    void InvalidateShapes(FFLiCharModel* pModel);

    void SetupRenderTextureBuffer(FFLiRenderTextureBuffer& renderTextureBuffer, const FFLCharModelDesc* pDesc, FFLiBufferAllocator* pAllocator) const;

private:
    FFLiCharModelCreateParam*   m_pCharModelCreateParam;
    FFLiManager*                m_pManager;
};
NN_STATIC_ASSERT(sizeof(FFLiCharModelCreator) == 8);

#endif // FFLI_CHAR_MODEL_CREATOR_H_
