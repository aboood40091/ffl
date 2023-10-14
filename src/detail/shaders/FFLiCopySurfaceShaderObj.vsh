#version 330 core

layout (location=0) in vec2 a_position;
layout (location=1) in vec2 a_texCoord;

out vec2 texCoord;

void main(void)
{
    gl_Position = vec4(a_position, 0, 1);
    texCoord = a_texCoord;
}
