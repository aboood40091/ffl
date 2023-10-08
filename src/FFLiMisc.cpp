#include <nn/ffl/FFLiDateTime.h>
#include <nn/ffl/FFLiMisc.h>

bool FFLiVerifyBirthday(s32 month, s32 day)
{
    bool ret = false;
    if (month == 0)
    {
        if (day == 0)
        {
            ret = true;
        }
    }
    else // if (month != 0)
    {
        if (day != 0 &&
            1 <= month && month <= 12 &&
            day <= FFLiGetMonthOfDay(month))
        {
            ret = true;
        }
    }
    return ret;
}
