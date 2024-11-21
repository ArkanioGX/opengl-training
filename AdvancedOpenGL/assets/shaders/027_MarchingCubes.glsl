#version 450 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct triangle
{
    vec3 p1;
    vec3 p2;
    vec3 p3;
};

struct voxel
{
    vec4 side[12];

};

layout (std430, binding = 1) buffer InputBlock
{
    int in_data[];
};

layout (std430, binding = 2) buffer OutputBlock
{
    voxel out_data[];
};

float tileSize = 0.5;

void main() {
    int global_id = int(gl_GlobalInvocationID.x) * 256 + int(gl_GlobalInvocationID.y) * 16 + int(gl_GlobalInvocationID.z);
    vec3 tilePos = vec3(gl_GlobalInvocationID.x*tileSize,gl_GlobalInvocationID.y*tileSize,gl_GlobalInvocationID.z*tileSize);

    out_data[global_id].side[0] = vec4(tilePos.x -0.25f,    tilePos.y + 0.25f,  tilePos.z -0.25f,1.0f);
    out_data[global_id].side[1] = vec4(tilePos.x -0.25f,    tilePos.y -0.25f,   tilePos.z -0.25f,1.0f);
    out_data[global_id].side[2] = vec4(tilePos.x + 0.25f,   tilePos.y -0.25f,   tilePos.z -0.25f,1.0f);
    //barrier();
}