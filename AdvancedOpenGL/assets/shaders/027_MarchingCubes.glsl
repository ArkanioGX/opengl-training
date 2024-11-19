#version 450 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding = 1) buffer InputBlock
{
    float in_data[];
};

void main() {
    barrier()
    in_data[0] += 10;
}