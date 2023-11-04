#include "menu.h"

#include "stb_image.h"

#include "soundManager.h"

extern SoundManager soundManager;

Menu::Menu()
{
    const vector<float> &temp = {
        1, 0, -10, 0, 0, 1, 1, 1,
        1, 1, -10, 0, 0, 1, 1, 0,
        0, 1, -10, 0, 0, 1, 0, 0,
        1, 0, -10, 0, 0, 1, 1, 1,
        0, 1, -10, 0, 0, 1, 0, 0,
        0, 0, -10, 0, 0, 1, 0, 1};

    for (int i = 0; i < temp.size(); i += 8)
    {
        vertices.push_back(glm::vec3(temp[i], temp[i + 1], temp[i + 2]));
        normals.push_back(glm::vec3(temp[i + 3], temp[i + 4], temp[i + 5]));
        uvs.push_back(glm::vec2(temp[i + 6], temp[i + 7]));
    }
    initPos();
    scale = glm::vec3(50, 50, 1);
    pos = glm::vec3(-25, -25, -30);

    const int tempBotton[4][4] = {
        {318, 407, 373, 109},
        {318, 565, 373, 117},
        {318, 720, 373, 126},
        {397, 914, 217, 53}};
    for (int i = 0; i < 4; i++)
        bottons.push_back(Botton(tempBotton[i][0], tempBotton[i][1], tempBotton[i][2], tempBotton[i][3]));
}

void Menu::render(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    model = glm::mat4(1.0);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Menu::initTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("res/menu.png", &width, &height, &nrChannels, 0);
    if (data)
        glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);
}

void Menu::initBuffer()
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

void Menu::update()
{
}

int Menu::isClicked(int x, int y)
{
    int type = 0;
    for (int i = 0; i < 4; i++)
        if (x > bottons[i].x && x < bottons[i].x + bottons[i].width && y > bottons[i].y && y < bottons[i].y + bottons[i].height)
        {
            type = i + 1;
            break;
        }

    if (type)
        soundManager.soundPlay(BUTTON_CLICK);

    return type;
}