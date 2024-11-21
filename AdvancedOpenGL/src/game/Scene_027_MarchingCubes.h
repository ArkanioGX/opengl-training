#ifndef Scene_027_MarchingCubes_H
#define Scene_027_MarchingCubes_H

#include "../engine/Scene.h"
#include "../engine/Assets.h"
#include "../engine/MeshObject.h"

constexpr int MAX_TRIS = 4096;
constexpr int VOXEL_SIDE = 16;
constexpr int VOXEL_TOTAL = VOXEL_SIDE * VOXEL_SIDE * VOXEL_SIDE;

struct ColorUInt32 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct Voxel{
    Vector4 tris[12];
};

class Scene_027_MarchingCubes : public Scene {
    public:
    Scene_027_MarchingCubes();
    ~Scene_027_MarchingCubes();
    void load();
    void clean();
    void pause();
    void resume();
    void handleEvent(const InputState &);
    void update(float dt);
    void draw();
    void setGame(Game *);

private:
    Game *game;
    float totalTime;

    ComputeShader computeShader;
    Shader renderShader;

    GLuint SSBO;
    GLuint SSBO2;
    GLuint GEOMETRY;
    GLuint VAO;
    GLuint VBO;

    Matrix4 proj_matrix;
    Matrix4 transform ;
    Matrix4 rotation;

    std::vector<Vector3> vertices;
};

#endif //Scene_027_MarchingCubes_H