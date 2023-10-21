#include "item.h"
#include "player.h"
#include "gameWorld.h"
#include "soundManager.h"
#include "camera.h"

#include "stb_image.h"

#ifndef __ITEM_STATIC__
#define __ITEM_STATIC__
std::random_device rd3;
std::mt19937 gen3(rd3());
std::uniform_int_distribution<int> dis3(0, 360);

GLint Item::object = -1;
vector<glm::vec3> Item::vertices;
vector<glm::vec3> Item::normals;
vector<glm::vec2> Item::uvs;

unsigned int Item::texture = -1;
#endif

extern Player player;
extern GameWorld gameWorld;
extern SoundManager soundManager;
extern Camera camera;

Item::Item(float posZ, float revolutionZ)
{
    if (object == -1)
    {
        object = objReader.loadObj("res/sphere.obj");
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
    setPosY(-1.0f);
    setPosZ(-posZ);
    setRevolutionZ(revolutionZ);
}

void Item::initTexture()
{
    if (texture != -1)
        return;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("res/sun.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Item::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);
    glGenBuffers(1, &tbo);

    // vertices
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    // texture
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
}

void Item::render(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    model = glm::mat4(1.0);
    model = glm::rotate(model, glm::radians(revolution.z), glm::vec3(0, 0, 1));
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.015, 0.015, 0.015));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, Item::texture);

    glDrawArrays(GL_TRIANGLES, 0, object);
}

void Item::colorInit()
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

void Item::update()
{
    collision();
    move();
}

void Item::move()
{
    setPosZ(pos.z + 0.03f);
    if (pos.z > 1.5)
        gameWorld.del_object(id);
}

void Item::collision()
{
    if (player.getPlayerDieTimer() != 0)
        return;
    if (abs(pos.z) < 0.3)
    {
        if (abs(revolution.z - player.getRevolution().z) < 15 || abs(revolution.z + 360.0f - player.getRevolution().z) < 15 || abs(revolution.z - 360.0f - player.getRevolution().z) < 15)
        {
            cout << "collision with Item" << endl;
            soundManager.soundPlay(ITEM_DESTROY);
            if (!player.getProtectedMode())
                camera.rolling(180.0f, 1);
            player.setProtectedMode(true);
            camera.setFovy(60.0f);
            camera.setEye(glm::vec3(0, 0, 0.4));
            gameWorld.del_object(id);
        }
    }
}