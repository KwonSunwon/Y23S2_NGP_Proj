#ifndef __BG_H__
#define __BG_H__

#include "object.h"

class BG : public Object
{
private:
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> uvs;

    static GLuint texture[2];

public:
    BG();
    void render(GLuint shaderProgramID) override;
    void initBuffer() override;
    void initTexture() override;
};

#endif