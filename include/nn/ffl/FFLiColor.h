#ifndef FFLI_COLOR_H_
#define FFLI_COLOR_H_

#include <nn/ffl/types.h>

bool FFLiUseOffScreenSrgbFetch();

struct FFLColor;

const FFLColor& FFLiGetSrgbFetchFacelineColor(s32 index);
const FFLColor& FFLiGetSrgbFetchBeardColor(s32 index);
const FFLColor& FFLiGetSrgbFetchEyebrowColor(s32 index);
const FFLColor& FFLiGetSrgbFetchMustacheColor(s32 index);
const FFLColor& FFLiGetSrgbFetchHairColor(s32 index);       // Deleted in NSMBU
const FFLColor& FFLiGetSrgbFetchEyeColorR(s32 index);
const FFLColor& FFLiGetSrgbFetchEyeColorG(s32 index);
const FFLColor& FFLiGetSrgbFetchEyeColorB(s32 index);
const FFLColor& FFLiGetSrgbFetchGlassColor(s32 index);      // Deleted in NSMBU
const FFLColor& FFLiGetSrgbFetchMouthColorR(s32 index);
const FFLColor& FFLiGetSrgbFetchMouthColorG(s32 index);
const FFLColor& FFLiGetSrgbFetchMouthColorB(s32 index);
const FFLColor& FFLiGetSrgbFetchMoleColor();
const FFLColor& FFLiGetSrgbFetchFavoriteColor(s32 index);   // Deleted in NSMBU
const FFLColor& FFLiGetSrgbFetchFaceLine();                 // Deleted in NSMBU
const FFLColor& FFLiGetSrgbFetchCapColor(s32 index);        // Deleted in NSMBU
const FFLColor& FFLiGetSrgbFetchNoselineColor();            // Deleted in NSMBU

const FFLColor& FFLiGetFacelineColor(s32 index);
const FFLColor& FFLiGetBeardColor(s32 index);               // Deleted in NSMBU
const FFLColor& FFLiGetEyebrowColor(s32 index);             // Deleted in NSMBU
const FFLColor& FFLiGetMustacheColor(s32 index);            // Deleted in NSMBU
const FFLColor& FFLiGetHairColor(s32 index);
const FFLColor& FFLiGetEyeColorR(s32 index);                // Deleted in NSMBU
const FFLColor& FFLiGetEyeColorG(s32 index);                // Deleted in NSMBU
const FFLColor& FFLiGetEyeColorB(s32 index);                // Deleted in NSMBU
const FFLColor& FFLiGetGlassColor(s32 index);
const FFLColor& FFLiGetMouthColorR(s32 index);              // Deleted in NSMBU
const FFLColor& FFLiGetMouthColorG(s32 index);              // Deleted in NSMBU
const FFLColor& FFLiGetMouthColorB(s32 index);              // Deleted in NSMBU
const FFLColor& FFLiGetMoleColor();                         // Deleted in NSMBU
const FFLColor& FFLiGetFavoriteColor(s32 index);
const FFLColor& FFLiGetFaceLine();
const FFLColor& FFLiGetCapColor(s32 index);
const FFLColor& FFLiGetNoselineColor();

/*
More deleted functions in NSMBU:

u32 FFLiGetColorNum(FFLiColorType type);
void FFLiSetColor(FFLColor color, FFLiColorType type, u32 index);
void FFLiGetColor(FFLColor* pColor, FFLiColorType type, u32 index);

void FFLiSetColorContainer(const FFLiColorContainer* pContainer, FFLiContainerType type);
void FFLiGetColorContainer(FFLiColorContainer* pContainer, FFLiContainerType type);

??? FFLiGetOffScreenSrgbFetch();
*/

#endif // FFLI_COLOR_H_
