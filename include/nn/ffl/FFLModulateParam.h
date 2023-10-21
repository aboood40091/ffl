#ifndef FFL_MODULATE_PARAM_H_
#define FFL_MODULATE_PARAM_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FFLModulateMode
{
    FFL_MODULATE_MODE_0 = 0,    // No Texture 
                                // Has Color (R)
                                // FragmentRGB = ColorR
                                // FragmentA = 1.0f
                                // Used by: ShapeBeard, ShapeNose, ShapeForehead, ShapeHair, Fill

    FFL_MODULATE_MODE_1 = 1,    // Has Texture
                                // No Color
                                // FragmentRGB = TextureRGB
                                // FragmentA = TextureA
                                // Used by: FaceMake, ShapeFaceline, ShapeMask

    FFL_MODULATE_MODE_2 = 2,    // Has Texture
                                // Has Color (R + G + B)
                                // FragmentRGB = ColorR * TextureR + ColorG * TextureG + ColorB * TextureB
                                // FragmentA = TextureA
                                // Used By: Mouth, Eye

    FFL_MODULATE_MODE_3 = 3,    // Has Texture
                                // Has Color (R)
                                // FragmentRGB = ColorR * TextureR
                                // FragmentA = TextureR
                                // Used by: Mustache, Eyebrow, Mole, FaceLine, FaceBeard, ShapeNoseline

    FFL_MODULATE_MODE_4 = 4,    // Has Texture
                                // Has Color (R)
                                // FragmentR = ColorR * TextureG
                                // FragmentA = TextureR
                                // Used by: ShapeGlass

    FFL_MODULATE_MODE_5 = 5     // Has Texture
                                // Has Color (R)
                                // FragmentRGB = ColorR * TextureR
                                // FragmentA = 1.0f
                                // Used by: ShapeCap
}
FFLModulateMode;

typedef enum FFLModulateType
{
    FFL_MODULATE_TYPE_SHAPE_FACELINE    =  0,
    FFL_MODULATE_TYPE_SHAPE_BEARD       =  1,
    FFL_MODULATE_TYPE_SHAPE_NOSE        =  2,
    FFL_MODULATE_TYPE_SHAPE_FOREHEAD    =  3,
    FFL_MODULATE_TYPE_SHAPE_HAIR        =  4,
    FFL_MODULATE_TYPE_SHAPE_CAP         =  5,
    FFL_MODULATE_TYPE_SHAPE_MASK        =  6,
    FFL_MODULATE_TYPE_SHAPE_NOSELINE    =  7,
    FFL_MODULATE_TYPE_SHAPE_GLASS       =  8,
    FFL_MODULATE_TYPE_MUSTACHE          =  9,
    FFL_MODULATE_TYPE_MOUTH             = 10,
    FFL_MODULATE_TYPE_EYEBROW           = 11,
    FFL_MODULATE_TYPE_EYE               = 12,
    FFL_MODULATE_TYPE_MOLE              = 13,
    FFL_MODULATE_TYPE_FACE_MAKE         = 14,
    FFL_MODULATE_TYPE_FACE_LINE         = 15,
    FFL_MODULATE_TYPE_FACE_BEARD        = 16,
    FFL_MODULATE_TYPE_FILL              = 17,

    FFL_MODULATE_TYPE_SHAPE_MAX         = FFL_MODULATE_TYPE_SHAPE_GLASS + 1
}
FFLModulateType;

typedef struct FFLColor FFLColor;

typedef struct FFLModulateParam
{
    FFLModulateMode     mode;
    FFLModulateType     type;
    const FFLColor*     pColorR;
    const FFLColor*     pColorG;
    const FFLColor*     pColorB;
    const GX2Texture*   pGX2Texture;
}
FFLModulateParam;
NN_STATIC_ASSERT(sizeof(FFLModulateParam) == 0x18);

#ifdef __cplusplus
}
#endif

#endif // FFL_MODULATE_PARAM_H_
