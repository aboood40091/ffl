#ifndef FFLI_MIIDATA_H_
#define FFLI_MIIDATA_H_

#include <nn/ffl/FFLResult.h>

struct  FFLiCharInfo;
class   FFLiMiiDataCore;
struct  FFLiMiiDataCoreRFL;
class   FFLiMiiDataOfficial;
struct  FFLiStoreDataCFL;

bool FFLiMiiDataCoreRFL2MiiDataCore(FFLiMiiDataCore* pMiiDataCore, const FFLiMiiDataCoreRFL& miiDataCoreRFL, bool replaceName);
void FFLiClearCreatorNameFromOfficial(FFLiMiiDataOfficial* pMiiDataOfficial);
void FFLiMiiDataOfficial2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataOfficial& miiDataOfficial);
FFLResult FFLiStoreDataCFLToCharInfo(FFLiCharInfo* pCharInfo, const FFLiStoreDataCFL& storeDataCFL);
FFLResult FFLiCharInfoToStoreDataCFL(FFLiStoreDataCFL* pStoreDataCFL, const FFLiCharInfo* pCharInfo);

#endif // FFLI_MIIDATA_H_
