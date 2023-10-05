#ifndef FFLI_DATE_TIME_H_
#define FFLI_DATE_TIME_H_

#include <nn/ffl/types.h>

struct FFLiDateTime
{
    s32 year;
    s32 month;
    s32 day;
    s32 hour;
    s32 minute;
    s32 second;
};
NN_STATIC_ASSERT(sizeof(FFLiDateTime) == 0x18);

s64 FFLiGetTick();

void FFLiGetDateTimeNow(FFLiDateTime* pDateTime);
void FFLiGetNowDateTime(s32* pYear, s32* pMonth, s32* pDay, s32* pHour, s32* pMinute, s32* pSecond);

bool FFLiIsLeapYear(s32 year);
s32 FFLiGetMonthOfDay(s32 month);
s32 FFLiDateToDays(s32 year, s32 month, s32 day);
s32 FFLiGetSecondsFrom_2000_01_01();

#endif // FFLI_DATE_TIME_H_
