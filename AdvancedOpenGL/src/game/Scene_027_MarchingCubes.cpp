#include "Scene_027_MarchingCubes.h"
#include "../engine/Timer.h"
#include "../engine/MacroUtils.h"
#include "../engine/Log.h"

#include <cstdlib>
#include <ctime>
#include <GL/glew.h>

Scene_027_MarchingCubes::Scene_027_MarchingCubes() : totalTime(0), frameIndex(0)
{
}

Scene_027_MarchingCubes::~Scene_027_MarchingCubes()
{
    clean();
}

void Scene_027_MarchingCubes::load()
{
    glEnable(GL_DEBUG_OUTPUT);

    Assets::loadComputeShader(SHADER_COMP(SHADER_NAME), SHADER_ID(SHADER_NAME));
    Assets::loadShader(SHADER_VERT(SHADER_NAME), SHADER_FRAG(SHADER_NAME), "", "", "", SHADER_ID(SHADER_NAME));

    computeShader = Assets::getComputeShader(SHADER_ID(SHADER_NAME));
    renderShader = Assets::getShader(SHADER_ID(SHADER_NAME));
    //------------------------------------------------------------------------------------------------------------------------
    // initial input data
    float input[4096] = {0};
    glGenBuffers(1, &SSBO);
    // bind buffer to binding point 1:
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
    // allocate buffer memory:
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(input), input, GL_STATIC_DRAW);
    // finished, unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    // the buffer is still bound at binding point 1, but not at the generic target "GL_SHADER_STORAGE_BUFFER"
    //------------------------------------------------------------------------------------------------------------------------
    // setup program
    //------------------------------------------------------------------------------------------------------------------------
    // the shader object will be deleted when it is no longer attached to any program
    //------------------------------------------------------------------------------------------------------------------------
    // invoke compute shader
    //------------------------------------------------------------------------------------------------------------------------

    glUseProgram(computeShader.id);
    glDispatchCompute(16, 16, 16);
    glUseProgram(0);

    // invokes the compute shader 1 x 1 x 1 = 10 times

    // each invocation can be identified by its "uvec3 gl_GlobalInvocationID"

    // in this case:

    // gl_GlobalInvocationID.x will be 0 ... 9

    // gl_GlobalInvocationID.y will be 0

    // gl_GlobalInvocationID.z will be 0

    //------------------------------------------------------------------------------------------------------------------------

    // show buffer data

    //------------------------------------------------------------------------------------------------------------------------

    float returned_data[4096] = { 0 };
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(returned_data), returned_data);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    for (unsigned int i = 0; i < 10; i++)
        std::cout << "returned_data[" << i << "] = " << returned_data[i] << std::endl;

    //------------------------------------------------------------------------------------------------------------------------

    // ...
    
/*
    std::vector<Vector3> triList(MAX_TRIS,Vector3(0,0,0));
    glGenBuffers(1, &SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(triList), triList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    
    glUseProgram(computeShader.id);
    glDispatchCompute(1, 1, 1);
    glUseProgram(0);

    std::vector<Vector3> returned_data;
    returned_data.resize(MAX_TRIS);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(returned_data), returned_data.data());

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);*/
    
}

void Scene_027_MarchingCubes::clean()
{
}

void Scene_027_MarchingCubes::pause()
{
}

void Scene_027_MarchingCubes::resume()
{
}

void Scene_027_MarchingCubes::handleEvent(const InputState &)
{
}

void Scene_027_MarchingCubes::update(float dt)
{
    totalTime += dt;
    float t = sin(totalTime);
    float fov = 80.f + (abs(1*t));
    transform = Matrix4::createTranslation(Vector3(0, 0, -8.0f));
    proj_matrix = Matrix4::createPerspectiveFOV(fov, game->windowWidth, game->windowHeight, 0.1f, 3000.0f);

    //computeShader.use();
    
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, flockBuffer[frameIndex]);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, flockBuffer[frameIndex ^ 1]);

    //glDispatchCompute(64, 1, 1);
}

void Scene_027_MarchingCubes::draw()
{
    renderShader.use();

    

    Matrix4 transform = Matrix4::createTranslation(Vector3(0, 0.f, -0.8f));
    renderShader.setMatrix4("mvp", proj_matrix*transform);

    glBindVertexArray(flockRenderVao[frameIndex]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, VOXEL_TOTAL);

    //frameIndex ^= 1;

    

}

void Scene_027_MarchingCubes::setGame(Game * _game)
{
    game = _game;
}
