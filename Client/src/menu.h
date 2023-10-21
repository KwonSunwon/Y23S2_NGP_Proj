#ifndef __MENU_H__
#define __MENU_H__

#include "object.h"

typedef struct Botton
{
    int x;
    int y;
    int width;
    int height;

    Botton(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
} Botton;

class Menu : public Object
{
private:
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> uvs;
    GLuint texture;

    vector<Botton> bottons;

public:
    Menu();
    void render(GLuint shaderProgramID) override;
    void initBuffer() override;
    void initTexture() override;
    void update() override;

    int isClicked(int x, int y);
};

#endif