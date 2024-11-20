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
    int side;
    vec3 pos;
    vec3 t1;
    vec3 t2;
    vec3 t3;
    vec3 t4;

};

layout (std430, binding = 1) buffer InputBlock
{
    voxel in_data[];
};


void main() {
    int global_id = int(gl_GlobalInvocationID.x) * 256 + int(gl_GlobalInvocationID.y) * 16 + int(gl_GlobalInvocationID.z);
    int local_id  = int(gl_LocalInvocationID.x);
    in_data[global_id].side = 1;
    in_data.pos = vec3(1.f,1.f,1.f);
    in_data.t1= vec3(1.f,1.f,1.f);
    in_data.t2 = vec3(1.f,1.f,1.f);
    in_data.t3 = vec3(1.f,1.f,1.f);
    in_data.t4 = vec3(1.f,1.f,1.f);
    //barrier();
}