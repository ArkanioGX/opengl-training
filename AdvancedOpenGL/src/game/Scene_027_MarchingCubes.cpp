#include "Scene_027_MarchingCubes.h"
#include "../engine/Timer.h"
#include "../engine/MacroUtils.h"
#include "../engine/Log.h"

#include <cstdlib>
#include <ctime>
#include <GL/glew.h>

Scene_027_MarchingCubes::Scene_027_MarchingCubes()
{
}

Scene_027_MarchingCubes::~Scene_027_MarchingCubes()
{
    clean();
}

void Scene_027_MarchingCubes::load()
{
    Assets::loadComputeShader(SHADER_COMP(SHADER_NAME), SHADER_ID(SHADER_NAME));
    Assets::loadShader(SHADER_VERT(SHADER_NAME), SHADER_FRAG(SHADER_NAME), "", "", "", SHADER_ID(SHADER_NAME));

    projection = Matrix4::createPerspectiveFOV(70.0f, game->windowWidth, game->windowHeight, 0.1f, 1000.0f);

    computeShader = Assets::getComputeShader(SHADER_ID(SHADER_NAME));
    renderShader = Assets::getShader(SHADER_ID(SHADER_NAME));

    glGenVertexArrays(1, &MCRenderVao);
    glBindVertexArray(MCRenderVao);
    static const GLfloat vertexPositions[] =
    {
        -1.f,  0.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f
    };

    // Generate data and put it in buffer object
    glGenBuffers(1, &MCBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, MCBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

    // Setup vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    transform = Matrix4::createTranslation(Vector3(0, 0, -4.0f));
    
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
}

void Scene_027_MarchingCubes::draw()
{
     static const GLfloat bgColor[] = {0.0f, 0.0f, 0.2f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, bgColor);
    renderShader.use();
    renderShader.setMatrix4("proj_matrix", projection);
    renderShader.setMatrix4("mv_matrix", transform);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Scene_027_MarchingCubes::setGame(Game * _game)
{
    game = _game;
}
