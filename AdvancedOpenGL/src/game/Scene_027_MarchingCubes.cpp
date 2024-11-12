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

    // This is position and normal data for a paper airplane
    static const Vector3 geometry[] =
    {
        // Positions
        Vector3(-0.5f, -0.5f, 0.f),
        Vector3(0.5f, -0.5f, 0.f),
        Vector3(-0.5f, 0.5f, 0.f),
        Vector3(-0.5f, 0.5f, 0.f),
        Vector3(0.5f, -0.5f, 0.f),
        Vector3(0.5f, 0.5f, 0.f),
    };

    computeShader = Assets::getComputeShader(SHADER_ID(SHADER_NAME));
    renderShader = Assets::getShader(SHADER_ID(SHADER_NAME));

    glGenBuffers(2, flockBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flockBuffer[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, VOXEL_TOTAL * sizeof(Voxel), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flockBuffer[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, VOXEL_TOTAL * sizeof(Voxel), NULL, GL_DYNAMIC_COPY);

    int i;

    glGenBuffers(1, &geometryBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometryBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);

    glGenVertexArrays(2, flockRenderVao);

    for (i = 0; i < 2; i++)
    {
        glBindVertexArray(flockRenderVao[i]);
        glBindBuffer(GL_ARRAY_BUFFER, geometryBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, flockBuffer[i]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), NULL);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel), (void *)sizeof(bool[8]));
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, flockBuffer[0]);
    Voxel * ptr = reinterpret_cast<Voxel *>(
        glMapBufferRange(GL_ARRAY_BUFFER, 0, VOXEL_TOTAL * sizeof(Voxel), 
                         GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)
    );

    for (i = 0; i < VOXEL_TOTAL; i++)
    {
        ptr[i].side[0] = false;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    transform = Matrix4::createTranslation(Vector3(0, 0, -2.0f));
    
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

    //computeShader.use();
    
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, flockBuffer[frameIndex]);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, flockBuffer[frameIndex ^ 1]);

    //glDispatchCompute(64, 1, 1);
}

void Scene_027_MarchingCubes::draw()
{
    renderShader.use();



    Matrix4 mv_matrix = Matrix4::createLookAt(Vector3(0.0f, 0.0f, -200.0f),
                                            Vector3(0.0f, 0.0f, 0.0f),
                                            Vector3(0.0f, 1.0f, 0.0f));
    Matrix4 proj_matrix = Matrix4::createPerspectiveFOV(60.0f, game->windowWidth, game->windowHeight, 0.1f, 3000.0f);
    Matrix4 mvp = proj_matrix * mv_matrix;

    renderShader.setMatrix4("mvp", mvp);

    glBindVertexArray(flockRenderVao[frameIndex]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, VOXEL_TOTAL);

    //frameIndex ^= 1;

    

}

void Scene_027_MarchingCubes::setGame(Game * _game)
{
    game = _game;
}
