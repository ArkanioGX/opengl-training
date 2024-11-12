#version 450 core

layout (location = 0) out vec4 color;

in VS_OUT
{
    vec4 color;
} fs_in;

void main()
{
    color = vec4(.8f,.0f,.7f,1.f);
} 
