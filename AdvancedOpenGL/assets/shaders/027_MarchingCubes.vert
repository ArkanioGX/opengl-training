#version 450 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3[12] tPos;

uniform mat4 mvp;


void main(void)
{
    gl_Position = mvp * vec4(position,1.f);
}
