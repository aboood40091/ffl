#ifndef FFLI_ADDITIONAL_INFO_H_
#define FFLI_ADDITIONAL_INFO_H_

#include <nn/ffl/FFLFontRegion.h>

struct FFLAdditionalInfo;

struct FFLiCharInfo;

void FFLiGetAdditionalInfo(FFLAdditionalInfo* pAdditionalInfo, const FFLiCharInfo* pCharInfo, BOOL checkFontRegion, FFLFontRegion fontRegion);

#endif // FFLI_ADDITIONAL_INFO_H_
