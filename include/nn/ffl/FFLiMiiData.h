#ifndef FFLI_MIIDATA_H_
#define FFLI_MIIDATA_H_

#include <nn/ffl/FFLResult.h>

struct  FFLiCharInfo;
class   FFLiMiiDataCore;
struct  FFLiMiiDataCoreRFL;
class   FFLiMiiDataOfficial;
struct  FFLiStoreDataCFL;

bool FFLiMiiDataCoreRFL2MiiDataCore(FFLiMiiDataCore* pMiiDataCore, const FFLiMiiDataCoreRFL& miiDataCoreRFL, bool replaceName);

bool FFLiMiiDataCoreRFL2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataCoreRFL& miiDataCoreRFL, const u16* pCreatorName, bool replaceName);
// bool FFLiMiiDataOfficialRFL2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataOfficialRFL& miiDataOfficialRFL, bool replaceName);   // Deleted in NSMBU

void FFLiClearCreatorNameFromOfficial(FFLiMiiDataOfficial* pMiiDataOfficial);

void FFLiMiiDataCore2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataCore& miiDataCore, const u16* pCreatorName, bool resetBirthday);
void FFLiCharInfo2MiiDataCore(FFLiMiiDataCore* pMiiDataCore, const FFLiCharInfo& charInfo, bool resetBirthday);

void FFLiMiiDataOfficial2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataOfficial& miiDataOfficial);
void FFLiCharInfo2MiiDataOfficial(FFLiMiiDataOfficial* pMiiDataOfficial, const FFLiCharInfo& charInfo, bool resetBirthday);

FFLResult FFLiStoreDataCFLToCharInfo(FFLiCharInfo* pCharInfo, const FFLiStoreDataCFL& storeDataCFL);
FFLResult FFLiCharInfoToStoreDataCFL(FFLiStoreDataCFL* pStoreDataCFL, const FFLiCharInfo* pCharInfo);

void FFLiMiiDataOfficialToStoreDataCFL(FFLiStoreDataCFL& storeDataCFL, const FFLiMiiDataOfficial& miiDataOfficial);

// FFLiStoreDataCFLToStoreData(FFLiStoreData& storeData, const FFLiStoreDataCFL& storeDataCFL); // Deleted in NSMBU

#endif // FFLI_MIIDATA_H_
