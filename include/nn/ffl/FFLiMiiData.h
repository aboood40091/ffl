#ifndef FFLI_MIIDATA_H_
#define FFLI_MIIDATA_H_

#include <nn/ffl/types.h>

struct  FFLiCharInfo;
class   FFLiMiiDataCore;
struct  FFLiMiiDataCoreRFL;
class   FFLiMiiDataOfficial;

bool FFLiMiiDataCoreRFL2MiiDataCore(FFLiMiiDataCore* pMiiDataCore, const FFLiMiiDataCoreRFL& miiDataCoreRFL, bool replaceName);
void FFLiClearCreatorNameFromOfficial(FFLiMiiDataOfficial* pMiiDataOfficial);
void FFLiMiiDataOfficial2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataOfficial& miiDataOfficial);

#endif // FFLI_MIIDATA_H_
