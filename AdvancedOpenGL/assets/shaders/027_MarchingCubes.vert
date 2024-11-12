#version 450 core

layout (location = 0) in vec3 position;

layout (location = 2) in vec3[12] vPos;;

uniform mat4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(position,0.f);
    vs_out.color = mix(C * 0.2, C, smoothstep(0.0, 0.8, abs(N).z));
}
