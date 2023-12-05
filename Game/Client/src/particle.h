#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "object.h"
#include "stdafx.h"
class Player;

class Particle : public Object
{
private:
    static GLint object;
    static vector<glm::vec3> vertices;
    static vector<glm::vec3> normals;
    static vector<glm::vec2> uvs;
    bool isPlayersParticle=true;
    float dx = 0, dy = 0, dz=0;

    static unsigned int texture;

protected:
public:
    Particle(bool, Player*);
    ~Particle();
    void render(GLuint shaderProgramID) override;
    void colorInit();
    void initBuffer() override;

    void update() override;
    void move();
    void collision();

    void setDx(float);
    void setDy(float);
    void setDz(float);
};

#endif