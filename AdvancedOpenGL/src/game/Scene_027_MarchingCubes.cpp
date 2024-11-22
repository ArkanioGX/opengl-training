#include "Scene_027_MarchingCubes.h"
#include "../engine/Timer.h"
#include "../engine/MacroUtils.h"
#include "../engine/Log.h"

#include <cstdlib>
#include <ctime>
#include <GL/glew.h>

Scene_027_MarchingCubes::Scene_027_MarchingCubes() : totalTime(0)
{
    //vertices.resize(600);
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

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
  
    computeShader = Assets::getComputeShader(SHADER_ID(SHADER_NAME));
    renderShader = Assets::getShader(SHADER_ID(SHADER_NAME));
    //bool input[] = {false, false, false, false, false,false, false, false, false, false};
    int input[64] = {};
    glGenBuffers(1, &SSBO);

    // bind buffer to binding point 1:
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);

    // allocate buffer memory:
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(input), input, GL_STATIC_DRAW);

    // finished, unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    Voxel output[64];
    glGenBuffers(1, &SSBO2);
    // bind buffer to binding point 1:

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBO2);
    // allocate buffer memory:
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(output), output, GL_STATIC_DRAW);

    // finished, unbind buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // the buffer is still bound at binding point 1, but not at the generic target "GL_SHADER_STORAGE_BUFFER"

    //------------------------------------------------------------------------------------------------------------------------

   

    // setup program

    //------------------------------------------------------------------------------------------------------------------------


    //------------------------------------------------------------------------------------------------------------------------

    // invoke compute shader

    //------------------------------------------------------------------------------------------------------------------------

    computeShader.use();
    glDispatchCompute(4, 4, 4);
    glUseProgram(0);

    // invokes the compute shader 10 x 1 x 1 = 10 times

    // each invocation can be identified by its "uvec3 gl_GlobalInvocationID"

    // in this case:

    // gl_GlobalInvocationID.x will be 0 ... 9

    // gl_GlobalInvocationID.y will be 0

    // gl_GlobalInvocationID.z will be 0

    //------------------------------------------------------------------------------------------------------------------------

    // show buffer data

    //------------------------------------------------------------------------------------------------------------------------
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO2);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(output), output);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);



    //vertices.resize(MAX_TRIS*3);
    for (int i = 0; i < sizeof(output)/sizeof(Voxel); i++){
        int index = 0;
        while (output[i].tris[index].w != 0 && index < 10){
            for (int j = 0; j < 3; j++){
                Vector3 vertex = Vector3(output[i].tris[index+j].x,output[i].tris[index+j].y,output[i].tris[index+j].z);
                vertices.push_back(vertex);
            }
            index += 3;
        }
    }

    

// Generate data and put it in buffer object
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  LOG(Info) << "vertexPositions size " << sizeof(vertices);

  // Setup vertex attribute
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);
    
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
    float ts = sin(totalTime);
    float tc = cos(totalTime);
    float fov = 70.f ;//+ (abs(1*ts));
    float dist = 10;
    Vector3 pos = Vector3(ts, 0, tc)*dist;
    rotation = Matrix4::createLookAt(pos,Vector3(0,0,0),Vector3::unitY);
    proj_matrix = Matrix4::createPerspectiveFOV(fov, game->windowWidth, game->windowHeight, 0.1f, 3000.0f);

    
}

void Scene_027_MarchingCubes::draw()
{
    static const GLfloat bgColor[] = {0.2f, 0.0f, 0.2f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, bgColor);

    renderShader.use();
    renderShader.setMatrix4("mvp", proj_matrix * rotation );
    glDrawElements(GL_TRIANGLES,sizeof(Vector3) * vertices.size(),GL_FLOAT_VEC3,0);
    //glDrawArrays(GL_TRIANGLES, 0, sizeof(Vector3) * vertices.size());

}

void Scene_027_MarchingCubes::setGame(Game * _game)
{
    game = _game;
}
