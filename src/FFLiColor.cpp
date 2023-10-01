#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiColor.h>

#include <cstring>

namespace {

enum FFLiColorType
{
    FFLI_COLOR_TYPE_FACELINE_COLOR_0    = 0,
    FFLI_COLOR_TYPE_HAIR_COLOR_0        = FFLI_COLOR_TYPE_FACELINE_COLOR_0  + FFLI_FACELINE_COLOR_NUM,
    FFLI_COLOR_TYPE_EYE_COLOR_R_0       = FFLI_COLOR_TYPE_HAIR_COLOR_0      + FFLI_HAIR_COLOR_NUM,
    FFLI_COLOR_TYPE_EYE_COLOR_G         = FFLI_COLOR_TYPE_EYE_COLOR_R_0     + FFLI_EYE_COLOR_R_NUM,
    FFLI_COLOR_TYPE_EYE_COLOR_B_0,   // = FFLI_COLOR_TYPE_EYE_COLOR_G       + 1,
    FFLI_COLOR_TYPE_GLASS_COLOR_0       = FFLI_COLOR_TYPE_EYE_COLOR_B_0     + FFLI_EYE_COLOR_B_NUM,
    FFLI_COLOR_TYPE_MOUTH_COLOR_R_0     = FFLI_COLOR_TYPE_GLASS_COLOR_0     + FFLI_GLASS_COLOR_NUM,
    FFLI_COLOR_TYPE_MOUTH_COLOR_G_0     = FFLI_COLOR_TYPE_MOUTH_COLOR_R_0   + FFLI_MOUTH_COLOR_R_NUM,
    FFLI_COLOR_TYPE_MOUTH_COLOR_B       = FFLI_COLOR_TYPE_MOUTH_COLOR_G_0   + FFLI_MOUTH_COLOR_G_NUM,
    FFLI_COLOR_TYPE_MOLE_COLOR,      // = FFLI_COLOR_TYPE_MOUTH_COLOR_B     + 1,
    FFLI_COLOR_TYPE_FAVORITE_COLOR_0,// = FFLI_COLOR_TYPE_MOLE_COLOR        + 1,
    FFLI_COLOR_TYPE_FACE_LINE           = FFLI_COLOR_TYPE_FAVORITE_COLOR_0  + FFLI_FAVORITE_COLOR_NUM,
    FFLI_COLOR_TYPE_MAX              // = FFLI_COLOR_TYPE_FACE_LINE         + 1
};
NN_STATIC_ASSERT(FFLI_COLOR_TYPE_MAX == 55);

enum FFLiContainerType
{
    FFLI_CONTAINER_TYPE_NORMAL  = 0,
    FFLI_CONTAINER_TYPE_SRGB    = 1,
    FFLI_CONTAINER_TYPE_MAX     = 2
};

struct FFLiColorContainer
{
    FFLColor    colors[FFLI_COLOR_TYPE_MAX];
};

FFLiColorContainer s_ColorContainer[FFLI_CONTAINER_TYPE_MAX];
FFLiContainerType s_ContainerType = FFLI_CONTAINER_TYPE_NORMAL;
bool s_UseOffScreenSrgbFetch = true;
bool s_IsIntializedColorContainer = false;

}

static bool UseOffScreenSrgbFetch()
{
    return s_UseOffScreenSrgbFetch;
}

bool FFLiUseOffScreenSrgbFetch()
{
    return UseOffScreenSrgbFetch() && s_ContainerType == FFLI_CONTAINER_TYPE_SRGB;
}

namespace {

const FFLiColorContainer& GetSrgbFetchColorContainer();
const FFLiColorContainer& GetColorContainer();

const FFLColor& GetFacelineColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetBeardColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetEyebrowColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetMustacheColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetHairColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetEyeColorR(const FFLiColorContainer& container, s32 index);
const FFLColor& GetEyeColorG(const FFLiColorContainer& container, s32 index);
const FFLColor& GetEyeColorB(const FFLiColorContainer& container, s32 index);
const FFLColor& GetGlassColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetMouthColorR(const FFLiColorContainer& container, s32 index);
const FFLColor& GetMouthColorG(const FFLiColorContainer& container, s32 index);
const FFLColor& GetMouthColorB(const FFLiColorContainer& container, s32 index);
const FFLColor& GetMoleColor(const FFLiColorContainer& container);
const FFLColor& GetFavoriteColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetFaceLine(const FFLiColorContainer& container);
const FFLColor& GetCapColor(const FFLiColorContainer& container, s32 index);
const FFLColor& GetNoselineColor(const FFLiColorContainer& container);

}

const FFLColor& FFLiGetSrgbFetchFacelineColor(s32 index)
{
    return GetFacelineColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchBeardColor(s32 index)
{
    return GetBeardColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchEyebrowColor(s32 index)
{
    return GetEyebrowColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchMustacheColor(s32 index)
{
    return GetMustacheColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchHairColor(s32 index)
{
    return GetHairColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchEyeColorR(s32 index)
{
    return GetEyeColorR(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchEyeColorG(s32 index)
{
    return GetEyeColorG(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchEyeColorB(s32 index)
{
    return GetEyeColorB(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchGlassColor(s32 index)
{
    return GetGlassColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchMouthColorR(s32 index)
{
    return GetMouthColorR(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchMouthColorG(s32 index)
{
    return GetMouthColorG(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchMouthColorB(s32 index)
{
    return GetMouthColorB(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchMoleColor()
{
    return GetMoleColor(GetSrgbFetchColorContainer());
}

const FFLColor& FFLiGetSrgbFetchFavoriteColor(s32 index)
{
    return GetFavoriteColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchFaceLine()
{
    return GetFaceLine(GetSrgbFetchColorContainer());
}

const FFLColor& FFLiGetSrgbFetchCapColor(s32 index)
{
    return GetCapColor(GetSrgbFetchColorContainer(), index);
}

const FFLColor& FFLiGetSrgbFetchNoselineColor()
{
    return GetNoselineColor(GetSrgbFetchColorContainer());
}

const FFLColor& FFLiGetFacelineColor(s32 index)
{
    return GetFacelineColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetBeardColor(s32 index)
{
    return GetBeardColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetEyebrowColor(s32 index)
{
    return GetEyebrowColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetMustacheColor(s32 index)
{
    return GetMustacheColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetHairColor(s32 index)
{
    return GetHairColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetEyeColorR(s32 index)
{
    return GetEyeColorR(GetColorContainer(), index);
}

const FFLColor& FFLiGetEyeColorG(s32 index)
{
    return GetEyeColorG(GetColorContainer(), index);
}

const FFLColor& FFLiGetEyeColorB(s32 index)
{
    return GetEyeColorB(GetColorContainer(), index);
}

const FFLColor& FFLiGetGlassColor(s32 index)
{
    return GetGlassColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetMouthColorR(s32 index)
{
    return GetMouthColorR(GetColorContainer(), index);
}

const FFLColor& FFLiGetMouthColorG(s32 index)
{
    return GetMouthColorG(GetColorContainer(), index);
}

const FFLColor& FFLiGetMouthColorB(s32 index)
{
    return GetMouthColorB(GetColorContainer(), index);
}

const FFLColor& FFLiGetMoleColor()
{
    return GetMoleColor(GetColorContainer());
}

const FFLColor& FFLiGetFavoriteColor(s32 index)
{
    return GetFavoriteColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetFaceLine()
{
    return GetFaceLine(GetColorContainer());
}

const FFLColor& FFLiGetCapColor(s32 index)
{
    return GetCapColor(GetColorContainer(), index);
}

const FFLColor& FFLiGetNoselineColor()
{
    return GetNoselineColor(GetColorContainer());
}

namespace {

void SetupColorContainer(FFLiColorContainer& container)
{
    const FFLColor facelineColor[FFLI_FACELINE_COLOR_NUM] = {
        { 1.000f, 0.827f, 0.678f, 1.000f },
        { 1.000f, 0.714f, 0.420f, 1.000f },
        { 0.870f, 0.475f, 0.259f, 1.000f },
        { 1.000f, 0.667f, 0.549f, 1.000f },
        { 0.678f, 0.318f, 0.161f, 1.000f },
        { 0.388f, 0.173f, 0.094f, 1.000f }
    };

    const FFLColor hairColor[FFLI_HAIR_COLOR_NUM] = {
        { 0.118f, 0.102f, 0.094f, 1.000f },
        { 0.251f, 0.125f, 0.063f, 1.000f },
        { 0.361f, 0.094f, 0.039f, 1.000f },
        { 0.486f, 0.227f, 0.078f, 1.000f },
        { 0.471f, 0.471f, 0.502f, 1.000f },
        { 0.306f, 0.243f, 0.063f, 1.000f },
        { 0.533f, 0.345f, 0.094f, 1.000f },
        { 0.816f, 0.627f, 0.290f, 1.000f }
    };

    const FFLColor eyeColorR[FFLI_EYE_COLOR_R_NUM] = {
        { 0.000f, 0.000f, 0.000f, 1.000f },
        { 1.000f, 0.510f, 0.000f, 1.000f },
        { 0.000f, 1.000f, 1.000f, 1.000f }
    };

    const FFLColor eyeColorG =
        { 1.000f, 1.000f, 1.000f, 1.000f };

    const FFLColor eyeColorB[FFLI_EYE_COLOR_B_NUM] = {
        { 0.000f, 0.000f, 0.000f, 1.000f },
        { 0.424f, 0.439f, 0.439f, 1.000f },
        { 0.400f, 0.235f, 0.173f, 1.000f },
        { 0.376f, 0.369f, 0.188f, 1.000f },
        { 0.275f, 0.329f, 0.659f, 1.000f },
        { 0.220f, 0.439f, 0.345f, 1.000f }
    };

    const FFLColor glassColor[FFLI_GLASS_COLOR_NUM] = {
        { 0.094f, 0.094f, 0.094f, 1.000f },
        { 0.376f, 0.219f, 0.062f, 1.000f },
        { 0.658f, 0.062f, 0.031f, 1.000f },
        { 0.125f, 0.188f, 0.407f, 1.000f },
        { 0.658f, 0.376f, 0.000f, 1.000f },
        { 0.470f, 0.439f, 0.407f, 1.000f },
    };

    const FFLColor mouthColorR[FFLI_MOUTH_COLOR_R_NUM] = {
        { 0.847f, 0.322f, 0.031f, 1.000f },
        { 0.941f, 0.047f, 0.031f, 1.000f },
        { 0.961f, 0.282f, 0.282f, 1.000f },
        { 0.941f, 0.604f, 0.455f, 1.000f },
        { 0.549f, 0.314f, 0.251f, 1.000f }
    };

    const FFLColor mouthColorG[FFLI_MOUTH_COLOR_G_NUM] = {
        { 0.510f, 0.188f, 0.094f, 1.000f },
        { 0.471f, 0.047f, 0.047f, 1.000f },
        { 0.533f, 0.125f, 0.157f, 1.000f },
        { 0.863f, 0.471f, 0.314f, 1.000f },
        { 0.275f, 0.118f, 0.039f, 1.000f }
    };

    const FFLColor mouthColorB =
        { 1.000f, 1.000f, 1.000f, 1.000f };

    const FFLColor moleColor =
        { 0.071f, 0.059f, 0.059f, 1.000f };

    const FFLColor favoriteColor[FFLI_FAVORITE_COLOR_NUM] = {
        { 0.824f, 0.118f, 0.078f, 1.000f },
        { 1.000f, 0.431f, 0.098f, 1.000f },
        { 1.000f, 0.847f, 0.125f, 1.000f },
        { 0.471f, 0.824f, 0.125f, 1.000f },
        { 0.000f, 0.471f, 0.188f, 1.000f },
        { 0.039f, 0.282f, 0.706f, 1.000f },
        { 0.235f, 0.667f, 0.871f, 1.000f },
        { 0.961f, 0.353f, 0.490f, 1.000f },
        { 0.451f, 0.157f, 0.678f, 1.000f },
        { 0.282f, 0.220f, 0.094f, 1.000f },
        { 0.878f, 0.878f, 0.878f, 1.000f },
        { 0.094f, 0.094f, 0.078f, 1.000f }
    };

    const FFLColor faceLine =
        { 0.000f, 0.000f, 0.000f, 1.000f };

    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_FACELINE_COLOR_0]),  facelineColor,  sizeof(facelineColor));
    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_HAIR_COLOR_0]),      hairColor,      sizeof(hairColor));
    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_EYE_COLOR_R_0]),     eyeColorR,      sizeof(eyeColorR));
    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_EYE_COLOR_B_0]),     eyeColorB,      sizeof(eyeColorB));
    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_GLASS_COLOR_0]),     glassColor,     sizeof(glassColor));
    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_MOUTH_COLOR_R_0]),   mouthColorR,    sizeof(mouthColorR));
    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_MOUTH_COLOR_G_0]),   mouthColorG,    sizeof(mouthColorG));
    std::memcpy(&(container.colors[FFLI_COLOR_TYPE_FAVORITE_COLOR_0]),  favoriteColor,  sizeof(favoriteColor));

    container.colors[FFLI_COLOR_TYPE_EYE_COLOR_G]   = eyeColorG;
    container.colors[FFLI_COLOR_TYPE_MOUTH_COLOR_B] = mouthColorB;
    container.colors[FFLI_COLOR_TYPE_MOLE_COLOR]    = moleColor;
    container.colors[FFLI_COLOR_TYPE_FACE_LINE]     = faceLine;
}

f32 SrgbFetchValue(f32 value)
{
    const u8 srgbFetchTable[256] = {
          0,   0,   0,   0,
          0,   0,   0,   1,
          1,   1,   1,   1,
          1,   1,   1,   1,
          1,   1,   2,   2,
          2,   2,   2,   2,
          2,   2,   3,   3,
          3,   3,   3,   3,
          4,   4,   4,   4,
          5,   5,   5,   5,
          5,   6,   6,   6,
          6,   7,   7,   7,
          8,   8,   8,   8,
          9,   9,   9,  10,
         10,  10,  11,  11,
         12,  12,  12,  13,
         13,  14,  14,  14,
         15,  15,  16,  16,
         17,  17,  17,  18,
         18,  19,  19,  20,
         20,  21,  22,  22,
         23,  23,  24,  24,
         25,  26,  26,  27,
         27,  28,  29,  29,
         30,  31,  31,  32,
         32,  33,  34,  35,
         35,  36,  37,  38,
         38,  39,  40,  41,
         41,  42,  43,  44,
         45,  45,  46,  47,
         48,  49,  50,  51,
         51,  52,  53,  54,
         55,  56,  57,  58,
         59,  60,  61,  62,
         63,  64,  65,  66,
         67,  68,  69,  70,
         71,  72,  73,  74,
         76,  77,  78,  79,
         80,  81,  82,  84,
         85,  86,  87,  89,
         90,  91,  92,  94,
         95,  96,  97,  99,
        100, 101, 103, 104,
        105, 107, 108, 109,
        111, 112, 114, 115,
        117, 118, 120, 121,
        122, 124, 125, 127,
        128, 129, 131, 132,
        134, 136, 137, 139,
        140, 142, 144, 145,
        147, 149, 150, 152,
        154, 155, 157, 159,
        160, 162, 164, 166,
        168, 169, 171, 173,
        175, 177, 178, 180,
        182, 184, 186, 188,
        190, 192, 194, 196,
        198, 200, 202, 204,
        206, 208, 210, 212,
        214, 216, 218, 220,
        222, 224, 226, 228,
        231, 233, 235, 237,
        239, 242, 244, 246,
        248, 250, 253, 255
    };

    return srgbFetchTable[u8(value * 255.0f)] / 255.0f;
}

void DegammaColorContainer(FFLiColorContainer& container)
{
    for (u32 i = 0; i < FFLI_COLOR_TYPE_MAX; i++)
    {
        FFLColor& color = container.colors[i];
        color.r = SrgbFetchValue(color.r);
        color.g = SrgbFetchValue(color.g);
        color.b = SrgbFetchValue(color.b);
    }
}

void InitializeColorContainerIfUninitialized()
{
    if (!s_IsIntializedColorContainer)
    {
        SetupColorContainer(s_ColorContainer[FFLI_CONTAINER_TYPE_NORMAL]);
        
        SetupColorContainer(s_ColorContainer[FFLI_CONTAINER_TYPE_SRGB]);
        DegammaColorContainer(s_ColorContainer[FFLI_CONTAINER_TYPE_SRGB]);

        s_IsIntializedColorContainer = true;
    }
}

const FFLiColorContainer& GetSrgbFetchColorContainer()
{
    InitializeColorContainerIfUninitialized();

    return
        s_UseOffScreenSrgbFetch
            ? s_ColorContainer[FFLI_CONTAINER_TYPE_NORMAL]
            : GetColorContainer();
}

const FFLiColorContainer& GetColorContainer()
{
    InitializeColorContainerIfUninitialized();
    return s_ColorContainer[s_ContainerType];
}

const FFLColor& GetFacelineColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_FACELINE_COLOR_0 + index];
}

const FFLColor& GetBeardColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_HAIR_COLOR_0 + index];
}

const FFLColor& GetEyebrowColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_HAIR_COLOR_0 + index];
}

const FFLColor& GetMustacheColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_HAIR_COLOR_0 + index];
}

const FFLColor& GetHairColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_HAIR_COLOR_0 + index];
}

const FFLColor& GetEyeColorR(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_EYE_COLOR_R_0 + 2];
}

const FFLColor& GetEyeColorG(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_EYE_COLOR_G];
}

const FFLColor& GetEyeColorB(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_EYE_COLOR_B_0 + index];
}

const FFLColor& GetGlassColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_GLASS_COLOR_0 + index];
}

const FFLColor& GetMouthColorR(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_MOUTH_COLOR_R_0 + index];
}

const FFLColor& GetMouthColorG(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_MOUTH_COLOR_G_0 + index];
}

const FFLColor& GetMouthColorB(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_MOUTH_COLOR_B];
}

const FFLColor& GetMoleColor(const FFLiColorContainer& container)
{
    return container.colors[FFLI_COLOR_TYPE_MOLE_COLOR];
}

const FFLColor& GetFavoriteColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_FAVORITE_COLOR_0 + index];
}

const FFLColor& GetFaceLine(const FFLiColorContainer& container)
{
    return container.colors[FFLI_COLOR_TYPE_FACE_LINE];
}

const FFLColor& GetCapColor(const FFLiColorContainer& container, s32 index)
{
    return container.colors[FFLI_COLOR_TYPE_FAVORITE_COLOR_0 + index];
}

const FFLColor& GetNoselineColor(const FFLiColorContainer& container)
{
    return container.colors[FFLI_COLOR_TYPE_FACE_LINE];
}

}