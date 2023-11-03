#if RIO_IS_CAFE

// ---------- FFLiCopySurfaceShaderObj Vertex Shader ----------

__attribute__((aligned(GX2_SHADER_PROGRAM_ALIGNMENT))) static const u32 FFLiCopySurfaceShaderObj_VS_shaderPtr[74] =
{
    0x00000000,0x00008009,0x20000000,0x00000ca0,    // 0x0000
    0x3ca00000,0x88060094,0x00400100,0x88040014,
    0x24000000,0x000000a0,0x00000000,0x00002000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x02001f00,0x900c4000,0x02041f00,0x900c4020,    // 0x0040
    0xf8001f00,0x900c2040,0xf9001f80,0x900c2060,
    0x00000080,0x000d0000
};

GX2AttribVar FFLiCopySurfaceShaderObj_VS_attrib_vars[] = {
    {"a_position", GX2_SHADER_VAR_TYPE_FLOAT2, 0, 0},
    {"a_texCoord", GX2_SHADER_VAR_TYPE_FLOAT2, 0, 1}
};



static GX2VertexShader FFLiCopySurfaceShaderObj_VS = {
  { // _regs
    0x00000103,0x00000000,0x00000000,0x00000001,    // 0x0000
    0xffffff00,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0x00000000,0xfffffffc,
    0x00000002,0x00000000,0x00000001,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x00000000,0x0000000e,0x00000010

  },
  296,
  (void *) FFLiCopySurfaceShaderObj_VS_shaderPtr,
  GX2_SHADER_MODE_UNIFORM_REGISTER,
  0,
  nullptr,
  0,
  nullptr,
  0,
  nullptr,
  0,
  nullptr,
  0,
  nullptr,
  2,
  FFLiCopySurfaceShaderObj_VS_attrib_vars,
  0,
  (GX2Boolean)0,
  {
    0x00000000,0x00000000,0x00000000,0x00000000     // 0x0000

  },
};


// ---------- FFLiCopySurfaceShaderObj Pixel Shader ----------

__attribute__((aligned(GX2_SHADER_PROGRAM_ALIGNMENT))) static const u32 FFLiCopySurfaceShaderObj_PS_shaderPtr[36] =
{
    0x10000000,0x0000c080,0x00000000,0x88062094,    // 0x0000
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x10000000,0x00100df0,0x00008010,0xecdfea0d

};

GX2SamplerVar FFLiCopySurfaceShaderObj_PS_sampler_vars[] = {
    {"s_texture", GX2_SAMPLER_VAR_TYPE_SAMPLER_2D, 0}
};



static GX2PixelShader FFLiCopySurfaceShaderObj_PS = {
  { // _regs
    0x00000001,0x00000002,0x14000001,0x00000000,    // 0x0000
    0x00000001,0x00000100,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x0000000f,0x00000001,0x00000010,
    0x00000000
  },
  144,
  (void *) FFLiCopySurfaceShaderObj_PS_shaderPtr,
  GX2_SHADER_MODE_UNIFORM_REGISTER,
  0,
  nullptr,
  0,
  nullptr,
  0,
  nullptr,
  0,
  nullptr,
  1,
  FFLiCopySurfaceShaderObj_PS_sampler_vars,
};

#elif RIO_IS_WIN

static const char* const FFLiCopySurfaceShaderObj_VS_src =
    "#version 330 core\n\n"

    "layout (location=0) in vec2 a_position;\n"
    "layout (location=1) in vec2 a_texCoord;\n\n"

    "out vec2 texCoord;\n\n"

    "void main(void)\n"
    "{\n"
    "    gl_Position = vec4(a_position, 0, 1);\n"
    "    texCoord = a_texCoord;\n"
    "}\n"
    ;

static const char* const FFLiCopySurfaceShaderObj_PS_src =
    "#version 330 core\n\n"

    "uniform sampler2D s_texture;\n\n"

    "in vec2 texCoord;\n\n"

    "out vec4 FragColor;\n\n"

    "void main(void)\n"
    "{\n"
    "    FragColor = texture(s_texture, texCoord);\n"
    "}\n"
    ;

#endif
