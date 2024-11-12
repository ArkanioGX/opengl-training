#version 450 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 16) in;

struct 
{
    bool[8] sides;
    float[12] tris;
};

layout (std430, binding = 0) readonly buffer members_in
{
    voxel voxelList[];
} input_data;

layout (std430, binding = 0) readonly buffer members_in
{
    voxel voxelList[];
} output_data;

shared flock_member shared_member[gl_WorkGroupSize.x*gl_WorkGroupSize.y*gl_WorkGroupSize.z];

void main(void)
{
    uint i, j;
    int global_id = int(gl_GlobalInvocationID.x);
    int local_id  = int(gl_LocalInvocationID.x);

    voxel me = input_data.voxelList[global_id];
    me.sides[0] = true;

    barrier();

    output_data.voxelList[global_id] = me;
}