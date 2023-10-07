#include <nn/ffl/FFLAge.h>
#include <nn/ffl/FFLRace.h>

#include <nn/ffl/FFLiCreateID.h>
#include <nn/ffl/FFLiDatabaseRandom.h>
#include <nn/ffl/FFLiMiddleDB.h>
#include <nn/ffl/FFLiMiiData.h>
#include <nn/ffl/FFLiRandomContext.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

#include <cstring>

FFLiDatabaseRandom::FFLiDatabaseRandom(FFLiRandomContext* pContext)
    : m_pRandomContext(pContext)
{
}

FFLiDatabaseRandom::~FFLiDatabaseRandom()
{
}

FFLResult FFLiDatabaseRandom::UpdateMiddleDB(FFLiMiddleDB* pMiddleDB)
{
    const FFLiMiddleDBRandomParam& param = pMiddleDB->RandomParam();

    while (!pMiddleDB->IsFull())
    {
        FFLiCharInfo charInfo;
        Get(&charInfo, param.Gender(), param.Age(), param.Race());
        FFLiGetTemporaryMiiID(&charInfo.createID);

        bool add = true;

        for (s32 i = 0; i < pMiddleDB->StoredSize(); i++)
        {
            FFLiCharInfo charInfoOfficial;
            FFLiMiiDataOfficial2CharInfo(&charInfoOfficial, pMiddleDB->Get(i));

            if (!FFLiCompareCharInfoWithAdditionalInfo(
                NULL,
                FFLI_COMPARE_CHAR_INFO_FLAGS_IN_COMPARE_PARTS,
                &charInfo, &charInfoOfficial,
                NULL, NULL
            ))
            {
                add = false;
                break;
            }
        }

        if (add)
            pMiddleDB->Add(charInfo);
    }

    return FFL_RESULT_OK;
}

void FFLiDatabaseRandom::DetermineParam(FFLGender* pGender, FFLAge* pAge, FFLRace* pRace)
{
    if (*pGender == FFL_GENDER_MAX)
    {
        u32 rnd = m_pRandomContext->Random(FFL_GENDER_MAX);
        *pGender =
            rnd == 0
                ? FFL_GENDER_MALE
                : FFL_GENDER_FEMALE;
    }

    if (*pAge == FFL_AGE_MAX)
    {
        u32 rnd = m_pRandomContext->Random(10);
        *pAge =
            rnd < 4
                ? FFL_AGE_0
                : (rnd < 8
                    ? FFL_AGE_1
                    : FFL_AGE_2);
    }

    if (*pRace == FFL_RACE_MAX)
    {
        u32 rnd = m_pRandomContext->Random(10);
        *pRace =
            rnd < 4
                ? FFL_RACE_2
                : (rnd < 8
                    ? FFL_RACE_1
                    : FFL_RACE_0);
    }
}

namespace {

struct RandomPartsArray
{
    u8  size;
    u8  data[47];
};
NN_STATIC_ASSERT(sizeof(RandomPartsArray) == 0x30);

static const RandomPartsArray RANDOM_PARTS_ARRAY_FACE_TYPE[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                10,
                { 0, 0, 1, 1, 2, 3, 4, 5, 9, 9 }
            },
            {
                10,
                { 0, 0, 1, 1, 2, 3, 4, 5, 9, 9 }
            },
            {
                10,
                { 0, 0, 1, 1, 2, 3, 4, 5, 9, 9 }
            }
        },
        {
            {
                12,
                { 0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 10, 11 }
            },
            {
                13,
                { 0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 10, 11 }
            },
            {
                12,
                { 0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 10, 11 }
            }
        },
        {
            {
                12,
                { 0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 10, 11 }
            },
            {
                13,
                { 0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 10, 11 }
            },
            {
                12,
                { 0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 10, 11 }
            }
        }
    },
    {
        {
            {
                10,
                { 0, 0, 1, 1, 2, 3, 4, 5, 9, 9 }
            },
            {
                10,
                { 0, 0, 1, 1, 2, 3, 4, 5, 9, 9 }
            },
            {
                10,
                { 0, 0, 1, 1, 2, 3, 4, 5, 9, 9 }
            }
        },
        {
            {
                12,
                { 0, 0, 0, 1, 1, 1, 2, 3, 4, 5, 8, 10 }
            },
            {
                12,
                { 0, 0, 0, 1, 1, 1, 2, 3, 4, 5, 8, 10 }
            },
            {
                12,
                { 0, 0, 0, 1, 1, 1, 2, 3, 4, 5, 8, 10 }
            }
        },
        {
            {
                12,
                { 0, 0, 0, 1, 1, 1, 2, 3, 4, 5, 8, 10 }
            },
            {
                12,
                { 0, 0, 0, 1, 1, 1, 2, 3, 4, 5, 8, 10 }
            },
            {
                12,
                { 0, 0, 0, 1, 1, 1, 2, 3, 4, 5, 8, 10 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_HAIR_TYPE[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                30,
                { 13, 23, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 54, 56, 57, 64, 66, 75, 76, 86, 89 }
            },
            {
                31,
                { 13, 23, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 54, 56, 57, 64, 66, 73, 75, 81, 86, 87 }
            },
            {
                31,
                { 13, 23, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 54, 56, 57, 64, 66, 73, 75, 81, 86, 87 }
            }
        },
        {
            {
                38,
                { 13, 23, 30, 31, 32, 33, 34, 36, 37, 38, 40, 42, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 59, 60, 64, 65, 66, 67, 68, 70, 75, 76, 86, 89 }
            },
            {
                39,
                { 13, 23, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 59, 60, 64, 65, 66, 67, 68, 70, 73, 75, 81, 86, 87 }
            },
            {
                39,
                { 13, 23, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 59, 60, 64, 65, 66, 67, 68, 70, 73, 75, 81, 86, 87 }
            }
        },
        {
            {
                18,
                { 13, 23, 30, 36, 37, 41, 45, 47, 51, 53, 54, 55, 58, 59, 65, 67, 86, 88 }
            },
            {
                19,
                { 13, 23, 30, 36, 37, 39, 41, 45, 47, 51, 53, 54, 55, 58, 59, 65, 67, 86, 88 }
            },
            {
                19,
                { 13, 23, 30, 36, 37, 39, 41, 45, 47, 51, 53, 54, 55, 58, 59, 65, 67, 86, 88 }
            }
        }
    },
    {
        {
            {
                39,
                { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 28, 46, 50, 61, 62, 63, 64, 69, 76, 77, 79, 80, 83, 85 }
            },
            {
                42,
                { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 28, 46, 50, 61, 62, 63, 64, 69, 72, 74, 77, 78, 82, 83, 84, 85, 87 }
            },
            {
                42,
                { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 28, 46, 50, 61, 62, 63, 64, 69, 72, 74, 77, 78, 82, 83, 84, 85, 87 }
            }
        },
        {
            {
                44,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 29, 42, 50, 58, 60, 62, 63, 64, 69, 71, 76, 79, 80, 81, 82, 83, 86 }
            },
            {
                44,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 29, 50, 58, 60, 62, 63, 64, 69, 71, 72, 74, 79, 81, 82, 83, 84, 85 }
            },
            {
                44,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 29, 50, 58, 60, 62, 63, 64, 69, 71, 72, 74, 79, 81, 82, 83, 84, 85 }
            }
        },
        {
            {
                24,
                { 0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 24, 25, 58, 62, 69, 76, 83 }
            },
            {
                27,
                { 0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 24, 25, 58, 62, 69, 74, 76, 81, 83, 85 }
            },
            {
                27,
                { 0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 24, 25, 58, 62, 69, 74, 76, 81, 83, 85 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_EYE_TYPE[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                26,
                { 2, 3, 5, 7, 8, 9, 11, 12, 13, 15, 16, 18, 27, 29, 32, 34, 36, 38, 39, 41, 43, 47, 49, 51, 53, 57 }
            },
            {
                26,
                { 2, 3, 5, 7, 8, 9, 11, 12, 13, 15, 16, 18, 27, 29, 32, 34, 36, 38, 39, 41, 43, 47, 49, 51, 53, 57 }
            },
            {
                27,
                { 2, 3, 5, 7, 8, 9, 11, 12, 13, 15, 16, 18, 26, 27, 29, 32, 34, 36, 38, 39, 41, 43, 47, 48, 49, 53, 57 }
            }
        },
        {
            {
                35,
                { 2, 3, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16, 17, 18, 21, 22, 27, 29, 31, 32, 34, 36, 37, 38, 39, 41, 43, 44, 47, 49, 51, 53, 55, 56, 57 }
            },
            {
                35,
                { 2, 3, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16, 17, 18, 21, 22, 27, 29, 31, 32, 34, 36, 37, 38, 39, 41, 43, 44, 47, 49, 51, 53, 55, 56, 57 }
            },
            {
                35,
                { 2, 3, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16, 18, 21, 22, 26, 27, 29, 31, 32, 34, 36, 37, 38, 39, 41, 43, 44, 47, 48, 49, 50, 53, 56, 57 }
            }
        },
        {
            {
                30,
                { 2, 3, 5, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 21, 22, 31, 32, 34, 36, 37, 39, 41, 44, 49, 51, 53, 55, 56, 57 }
            },
            {
                30,
                { 2, 3, 5, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 21, 22, 31, 32, 34, 36, 37, 39, 41, 44, 49, 51, 53, 55, 56, 57 }
            },
            {
                30,
                { 2, 3, 5, 7, 8, 9, 11, 12, 13, 14, 15, 16, 18, 21, 22, 26, 31, 32, 34, 36, 37, 39, 41, 44, 48, 49, 50, 51, 53, 57 }
            }
        }
    },
    {
        {
            {
                39,
                { 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47, 48, 53, 54, 57, 59 }
            },
            {
                39,
                { 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47, 48, 53, 54, 57, 59 }
            },
            {
                40,
                { 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 26, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47, 48, 53, 54, 57, 59 }
            }
        },
        {
            {
                46,
                { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 27, 28, 29, 30, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 45, 46, 47, 48, 53, 54, 57, 58, 59 }
            },
            {
                46,
                { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 27, 28, 29, 30, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 45, 46, 47, 48, 53, 54, 57, 58, 59 }
            },
            {
                46,
                { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 45, 46, 47, 48, 53, 54, 57, 58, 59 }
            }
        },
        {
            {
                34,
                { 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47 }
            },
            {
                34,
                { 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47 }
            },
            {
                35,
                { 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 23, 24, 25, 26, 27, 28, 29, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 46, 47 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_EYEBROW_TYPE[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                18,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 17, 18, 20 }
            },
            {
                18,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 17, 18, 20 }
            },
            {
                18,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 17, 18, 20 }
            }
        },
        {
            {
                23,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 }
            },
            {
                23,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 }
            },
            {
                23,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 }
            }
        },
        {
            {
                21,
                { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22 }
            },
            {
                21,
                { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22 }
            },
            {
                21,
                { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22 }
            }
        }
    },
    {
        {
            {
                9,
                { 0, 1, 3, 7, 8, 9, 10, 11, 13 }
            },
            {
                9,
                { 0, 1, 3, 7, 8, 9, 10, 11, 13 }
            },
            {
                9,
                { 0, 1, 3, 7, 8, 9, 10, 11, 13 }
            }
        },
        {
            {
                11,
                { 0, 1, 3, 7, 8, 9, 10, 11, 13, 15, 19 }
            },
            {
                11,
                { 0, 1, 3, 7, 8, 9, 10, 11, 13, 15, 19 }
            },
            {
                11,
                { 0, 1, 3, 7, 8, 9, 10, 11, 13, 15, 19 }
            }
        },
        {
            {
                9,
                { 0, 3, 7, 8, 9, 10, 11, 13, 15 }
            },
            {
                9,
                { 0, 3, 7, 8, 9, 10, 11, 13, 15 }
            },
            {
                9,
                { 0, 3, 7, 8, 9, 10, 11, 13, 15 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_NOSE_TYPE[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                11,
                { 0, 1, 2, 3, 4, 5, 7, 8, 10, 13, 14 }
            },
            {
                11,
                { 0, 1, 2, 3, 4, 5, 7, 8, 10, 13, 14 }
            },
            {
                11,
                { 0, 1, 2, 3, 4, 5, 7, 8, 10, 13, 14 }
            }
        },
        {
            {
                18,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }
            },
            {
                18,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }
            },
            {
                15,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16 }
            }
        },
        {
            {
                18,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }
            },
            {
                18,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }
            },
            {
                15,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16 }
            }
        }
    },
    {
        {
            {
                8,
                { 0, 1, 3, 4, 8, 10, 13, 14 }
            },
            {
                8,
                { 0, 1, 3, 4, 8, 10, 13, 14 }
            },
            {
                8,
                { 0, 1, 3, 4, 8, 10, 13, 14 }
            }
        },
        {
            {
                12,
                { 0, 1, 3, 4, 6, 8, 9, 10, 11, 13, 14, 15 }
            },
            {
                11,
                { 0, 1, 3, 4, 6, 8, 9, 10, 11, 13, 15 }
            },
            {
                10,
                { 0, 1, 3, 4, 6, 8, 10, 11, 13, 14 }
            }
        },
        {
            {
                12,
                { 0, 1, 3, 4, 6, 8, 9, 10, 11, 13, 14, 15 }
            },
            {
                11,
                { 0, 1, 3, 4, 6, 8, 9, 10, 11, 13, 15 }
            },
            {
                10,
                { 0, 1, 3, 4, 6, 8, 10, 11, 13, 14 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_MOUTH_TYPE[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                25,
                { 0, 2, 3, 6, 7, 8, 9, 10, 12, 14, 15, 17, 18, 19, 21, 22, 23, 25, 26, 28, 30, 32, 33, 34, 35 }
            },
            {
                27,
                { 0, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23, 25, 26, 28, 30, 32, 33, 34, 35 }
            },
            {
                28,
                { 0, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23, 25, 26, 28, 30, 31, 32, 33, 34, 35 }
            }
        },
        {
            {
                24,
                { 0, 2, 3, 6, 7, 8, 9, 10, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 30, 31, 33, 34, 35 }
            },
            {
                26,
                { 0, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 30, 31, 33, 34, 35 }
            },
            {
                26,
                { 0, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 30, 31, 33, 34, 35 }
            }
        },
        {
            {
                24,
                { 0, 2, 3, 6, 7, 8, 9, 10, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 30, 31, 33, 34, 35 }
            },
            {
                26,
                { 0, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 30, 31, 33, 34, 35 }
            },
            {
                26,
                { 0, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 30, 31, 33, 34, 35 }
            }
        }
    },
    {
        {
            {
                25,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 14, 15, 17, 18, 19, 21, 22, 23, 25, 26, 30, 33, 34, 35 }
            },
            {
                26,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23, 25, 26, 30, 33, 34, 35 }
            },
            {
                26,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23, 25, 26, 30, 33, 34, 35 }
            }
        },
        {
            {
                25,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 14, 15, 17, 18, 19, 21, 22, 23, 24, 26, 27, 29, 33, 35 }
            },
            {
                26,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23, 24, 26, 27, 29, 33, 35 }
            },
            {
                25,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 17, 18, 19, 21, 22, 23, 24, 25, 29, 33, 35 }
            }
        },
        {
            {
                24,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 29, 33 }
            },
            {
                25,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 29, 33 }
            },
            {
                25,
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 29, 33 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_FACE_LINE[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8 }
            }
        },
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
            }
        },
        {
            {
                20,
                { 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11 }
            },
            {
                20,
                { 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11 }
            },
            {
                20,
                { 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11 }
            }
        }
    },
    {
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 }
            }
        },
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 4, 8, 8 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 4, 8, 8 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 4 }
            }
        },
        {
            {
                20,
                { 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11 }
            },
            {
                20,
                { 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11 }
            },
            {
                20,
                { 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_FACE_MAKEUP[FFL_GENDER_MAX][FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9 }
            }
        },
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9 }
            }
        },
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9 }
            }
        }
    },
    {
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 9, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 9, 9 }
            }
        },
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 8, 9 }
            },
            {
                20,
                { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 9 }
            }
        },
        {
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
            },
            {
                20,
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
            }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_FACELINE_COLOR[FFL_GENDER_MAX][FFL_RACE_MAX] = {
    {
        {
            10,
            { 2, 2, 4, 4, 4, 4, 5, 5, 5, 5 }
        },
        {
            10,
            { 0, 0, 0, 0, 1, 1, 2, 3, 3, 3 }
        },
        {
            10,
            { 0, 0, 1, 1, 1, 1, 1, 1, 1, 2 }
        }
    },
    {
        {
            10,
            { 2, 2, 4, 4, 4, 4, 5, 5, 5, 5 }
        },
        {
            10,
            { 0, 0, 0, 0, 0, 0, 0, 0, 1, 3 }
        },
        {
            10,
            { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_HAIR_COLOR[FFL_AGE_MAX][FFL_RACE_MAX] = {
    {
        {
            20,
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
        },
        {
            20,
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
        },
        {
            20,
            { 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }
        }
    },
    {
        {
            20,
            { 2, 3, 3, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7 }
        },
        {
            20,
            { 2, 3, 3, 3, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7 }
        },
        {
            20,
            { 2, 3, 3, 4, 4, 4, 4, 4, 4, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7 }
        }
    },
    {
        {
            20,
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 }
        },
        {
            20,
            { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 3, 3, 3 }
        },
        {
            20,
            { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }
        }
    }
};

static const RandomPartsArray RANDOM_PARTS_ARRAY_EYE_COLOR[FFL_RACE_MAX] = {
    {
        10,
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }
    },
    {
        10,
        { 0, 1, 1, 2, 3, 3, 4, 4, 4, 5 }
    },
    {
        10,
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }
    }
};

u8 GetRandomParts(const RandomPartsArray& array, FFLiRandomContext* pContext)
{
    return array.data[pContext->Random(array.size)];
}

static const s32 RANDOM_GLASS_TYPE[FFL_AGE_MAX][FFL_GLASS_TYPE_MAX] = {
    {  90,  94,  96, 100,   0,   0,   0,   0,   0 },
    {  83,  86,  90,  93,  94,  96,  98, 100,   0 },
    {  78,  83,   0,  93,   0,   0,  98, 100,   0 }
};

s32 GetRandomGlassType(FFLAge age, FFLiRandomContext* pContext)
{
    s32 target = pContext->Random(100);

    s32 type = 0;
    while (target >= RANDOM_GLASS_TYPE[age][type])
        type++;

    return type;
}

}

void FFLiDatabaseRandom::Get(FFLiCharInfo* pCharInfo, FFLGender gender, FFLAge age, FFLRace race)
{
    DetermineParam(&gender, &age, &race);

    pCharInfo->miiVersion = 3;

    s32 basePositionY = 0;
    if (gender == FFL_GENDER_FEMALE || age == FFL_AGE_0)
        basePositionY = m_pRandomContext->Random(3);

    pCharInfo->parts.faceType = GetRandomParts(RANDOM_PARTS_ARRAY_FACE_TYPE[gender][age][race], m_pRandomContext);
    pCharInfo->parts.facelineColor = GetRandomParts(RANDOM_PARTS_ARRAY_FACELINE_COLOR[gender][race], m_pRandomContext);
    pCharInfo->parts.faceLine = GetRandomParts(RANDOM_PARTS_ARRAY_FACE_LINE[gender][age][race], m_pRandomContext);
    pCharInfo->parts.faceMakeup = GetRandomParts(RANDOM_PARTS_ARRAY_FACE_MAKEUP[gender][age][race], m_pRandomContext);

    pCharInfo->parts.hairType = GetRandomParts(RANDOM_PARTS_ARRAY_HAIR_TYPE[gender][age][race], m_pRandomContext);
    pCharInfo->parts.hairColor = GetRandomParts(RANDOM_PARTS_ARRAY_HAIR_COLOR[race][age], m_pRandomContext);
    pCharInfo->parts.hairDir = m_pRandomContext->Random(FFL_HAIR_DIR_MAX);

    pCharInfo->parts.eyeType = GetRandomParts(RANDOM_PARTS_ARRAY_EYE_TYPE[gender][age][race], m_pRandomContext);
    pCharInfo->parts.eyeColor = GetRandomParts(RANDOM_PARTS_ARRAY_EYE_COLOR[race], m_pRandomContext);
    pCharInfo->parts.eyeScaleX = 4;
    pCharInfo->parts.eyeScaleY = 3;
    s32 eyeRotateOffsetTarget;
    if (gender == FFL_GENDER_MALE)
    {
        pCharInfo->parts.eyeRotateOffset = 4;
        eyeRotateOffsetTarget = FFLiiGetEyeRotateOffset(2);
    }
    else
    {
        pCharInfo->parts.eyeRotateOffset = 3;
        eyeRotateOffsetTarget = FFLiiGetEyeRotateOffset(4);
    }
    s32 eyeRotateOffsetBase = FFLiiGetEyeRotateOffset(pCharInfo->parts.eyeType);
    pCharInfo->parts.eyeSpacingX = 2;
    pCharInfo->parts.eyePositionY = basePositionY + 12;
    pCharInfo->parts.eyeRotateOffset += eyeRotateOffsetTarget - eyeRotateOffsetBase;

    pCharInfo->parts.eyebrowType = GetRandomParts(RANDOM_PARTS_ARRAY_EYEBROW_TYPE[gender][age][race], m_pRandomContext);
    pCharInfo->parts.eyebrowColor = pCharInfo->parts.hairColor;
    pCharInfo->parts.eyebrowScaleX = 4;
    pCharInfo->parts.eyebrowScaleY = 3;
    pCharInfo->parts.eyebrowRotateOffset = 6;
    pCharInfo->parts.eyebrowSpacingX = 2;
    s32 eyebrowRotateOffsetTarget;
    if (race == FFL_RACE_2)
    {
        pCharInfo->parts.eyebrowPositionY = basePositionY + 9;
        eyebrowRotateOffsetTarget = FFLiiGetEyebrowRotateOffset(6);
    }
    else
    {
        pCharInfo->parts.eyebrowPositionY = basePositionY + 10;
        eyebrowRotateOffsetTarget = FFLiiGetEyebrowRotateOffset(0);
    }
    s32 eyebrowRotateOffsetBase = FFLiiGetEyebrowRotateOffset(pCharInfo->parts.eyebrowType);
    pCharInfo->parts.eyebrowRotateOffset += eyebrowRotateOffsetTarget - eyebrowRotateOffsetBase;

    pCharInfo->parts.noseType = GetRandomParts(RANDOM_PARTS_ARRAY_NOSE_TYPE[gender][age][race], m_pRandomContext);
    pCharInfo->parts.noseScale = gender == FFL_GENDER_MALE ? 4 : 3;
    pCharInfo->parts.nosePositionY = basePositionY + 9;

    pCharInfo->parts.mouthType = GetRandomParts(RANDOM_PARTS_ARRAY_MOUTH_TYPE[gender][age][race], m_pRandomContext);
    pCharInfo->parts.mouthColor = gender == FFL_GENDER_MALE ? 0 : m_pRandomContext->Random(FFL_MOUTH_COLOR_MAX);
    pCharInfo->parts.mouthScaleX = 4;
    pCharInfo->parts.mouthScaleY = 3;
    pCharInfo->parts.mouthPositionY = basePositionY + 13;

    s32 mustacheType, beardType, mustachePositionY;
    if ((age == FFL_AGE_1 || age == FFL_AGE_2) && m_pRandomContext->Random(10) < 2)
    {
        mustacheType = 0;
        bool randomBeardType = false;
        switch (m_pRandomContext->Random(3))
        {
        case 0:
            randomBeardType = true;
            break;
        case 2:
            randomBeardType = true; // fall-through
        case 1:
            mustacheType = m_pRandomContext->Random(5) + 1;
            break;
        }
        beardType = randomBeardType ? m_pRandomContext->Random(5) + 1 : 0;
        mustachePositionY = 10;
    }
    else
    {
        mustacheType = 0;
        beardType = 0;
        mustachePositionY = basePositionY + 10;
    }
    pCharInfo->parts.mustacheType = mustacheType;
    pCharInfo->parts.beardType = beardType;
    pCharInfo->parts.beardColor = pCharInfo->parts.hairColor;
    pCharInfo->parts.mustacheScale = 4;
    pCharInfo->parts.mustachePositionY = mustachePositionY;

    pCharInfo->parts.glassType = GetRandomGlassType(age, m_pRandomContext);
    pCharInfo->parts.glassColor = 0;
    pCharInfo->parts.glassScale = 4;
    pCharInfo->parts.glassPositionY = basePositionY + 10;

    pCharInfo->parts.moleType = 0;
    pCharInfo->parts.moleScale = 4;
    pCharInfo->parts.molePositionX = 2;
    pCharInfo->parts.molePositionY = 20;

    pCharInfo->height = 64;
    pCharInfo->build = 64;

    std::memset(pCharInfo->name, 0, sizeof(u16) * (10 + 1));
    FFLiCopyWcharT2U16(pCharInfo->name, 10 + 1, L"no name");

    std::memset(pCharInfo->creatorName, 0, sizeof(u16) * (10 + 1));

    pCharInfo->gender = gender;

    pCharInfo->birthMonth = 0;
    pCharInfo->birthDay = 0;

    pCharInfo->favoriteColor = m_pRandomContext->Random(FFL_FAVORITE_COLOR_MAX);

    pCharInfo->favoriteMii = false;
    pCharInfo->copyable = true;
    pCharInfo->ngWord = false;
    pCharInfo->localOnly = false;

    pCharInfo->regionMove = 0;
    pCharInfo->fontRegion = FFL_FONT_REGION_0;

    pCharInfo->pageIndex = 0;
    pCharInfo->slotIndex = 0;

    pCharInfo->birthPlatform = FFL_BIRTH_PLATFORM_WII_U;

    pCharInfo->_114 = 0;

    std::memset(&pCharInfo->authorID, 0, sizeof(FFLiAuthorID));
}
