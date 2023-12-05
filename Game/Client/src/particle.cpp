#include "particle.h"
#include "player.h"
#include "gameWorld.h"

#ifndef __PARTICLE_STATIC__
#define __PARTICLE_STATIC__
std::random_device rd1;
std::mt19937 gen1(rd1());
std::uniform_int_distribution<int> dis1(0, 100);

GLint Particle::object = -1;
vector<glm::vec3> Particle::vertices;
vector<glm::vec3> Particle::normals;
vector<glm::vec2> Particle::uvs;

unsigned int Particle::texture = -1;
#endif
extern Player player;
extern GameWorld gameWorld;

Particle::Particle(bool isPlayersParticle, Player* p)
{
    if (object == -1)
    {
        object = objReader.loadObj("res/wall.obj");
        vertices.resize(objReader.out_vertices.size());
        normals.resize(objReader.out_normals.size());
        uvs.resize(objReader.out_uvs.size());
        for (int i = 0; i < objReader.out_vertices.size(); i++)
        {
            vertices[i] = objReader.out_vertices[i];
            normals[i] = objReader.out_normals[i];
            uvs[i] = objReader.out_uvs[i];
        }
    }
    
    setPosX(p->getPos().x);
    setPosY(p->getPos().y);
    setPosZ(p->getPos().z);
    if (isPlayersParticle)
    {
        setDx((float)(dis1(gen1) - 50) / 10000);
        setDy((float)(dis1(gen1) - 50) / 25000);
        setDz((float)(dis1(gen1)) / 500);
    }
    else
    {
        setDx((float)(dis1(gen1) - 50) / 1000);
        setDy((float)(dis1(gen1) - 50) / 1000);
        setDz((float)(dis1(gen1) - 50) / 1000);
    }

    this->isPlayersParticle = isPlayersParticle;
}

Particle::~Particle()
{
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1, &nbo);
    glDeleteVertexArrays(1, &vao);
}

void Particle::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);

    // vertices
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
}

void Particle::render(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    model = glm::mat4(1.0);
    //model = glm::rotate(model, glm::radians(revolution.z), glm::vec3(0, 0, 1));
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, object);
}

void Particle::colorInit()
{
    for (int i = 0; i < vertices.size(); i++)
    {
        colors.push_back(0.5f);
        colors.push_back(0.3f);
        colors.push_back(0.7f);
    }

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(2);
}

void Particle::update()
{
    collision();
    move();
}

void Particle::move()
{
    if (isPlayersParticle)
    {
        setPosZ(pos.z + dz);
        setPosY(pos.y + dy);
        setPosX(pos.x + dx);
        if (pos.z > 3.0)
        {
            //setPosY(-1.0f);
           /* setPosX(player.getPos().x);
            setPosZ(player.getPos().z);
            setRevolutionZ(player.getRevolution().z);*/
        }
    }
    else
    {
        setPosZ(pos.z + dz);
        setPosY(pos.y + dy);
        setPosX(pos.x + dx);
        if (abs(pos.z) > 3.0 || abs(pos.x)>3.0||abs(pos.y>3.0))
        {
            //gameWorld.del_object(id);
            //delete this;

        }
    }
        
}

void Particle::setDx(float in)
{
    dx = in;
}
void Particle::setDy(float in)
{
    dy = in;
}
void Particle::setDz(float in)
{
    dz = in;
}
void Particle::collision()
{
    
}