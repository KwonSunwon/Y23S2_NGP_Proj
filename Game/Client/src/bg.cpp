#include "bg.h"
#include "stb_image.h"

#include "player.h"

#ifndef __BG_STATIC__
#define __BG_STATIC__
GLuint BG::texture[3] = { -1, -1, -1 };
#endif

extern Player player;

BG::BG()
{
	const vector<float>& temp = {
		1, 0, -10, 0, 0, 1, 1, 1,
		1, 1, -10, 0, 0, 1, 1, 0,
		0, 1, -10, 0, 0, 1, 0, 0,
		1, 0, -10, 0, 0, 1, 1, 1,
		0, 1, -10, 0, 0, 1, 0, 0,
		0, 0, -10, 0, 0, 1, 0, 1 };

	for (int i = 0; i < temp.size(); i += 8)
	{
		vertices.push_back(glm::vec3(temp[i], temp[i + 1], temp[i + 2]));
		normals.push_back(glm::vec3(temp[i + 3], temp[i + 4], temp[i + 5]));
		uvs.push_back(glm::vec2(temp[i + 6], temp[i + 7]));
	}

	initPos();
	scale = glm::vec3(300, 300, 1);
	pos = glm::vec3(-150, -150, -8);
}

void BG::render(GLuint ID)
{
	glUseProgram(ID);

	model = glm::mat4(1.0);
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);

	/*if (!player.getProtectedMode())
	{
		scale = glm::vec3(100, 100, 1);
		pos = glm::vec3(-50, -50, -8);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	else
	{
		scale = glm::vec3(500, 500, 1);
		pos = glm::vec3(-250, -250, -8);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	}*/
	switch (type)
	{
	case BG_TYPE::MAIN:
		scale = glm::vec3(100, 100, 1);
		pos = glm::vec3(-50, -50, -8);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		break;
	case BG_TYPE::WIN:
		scale = glm::vec3(220, 220, 1);
		pos = glm::vec3(-110, -110, -50);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		break;
	case BG_TYPE::LOOSE:
		scale = glm::vec3(220, 220, 1);
		pos = glm::vec3(-110, -110, -50);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		break;
	default:
		break;
	}

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void BG::initTexture()
{
	if (texture[0] != -1)
		return;

	glGenTextures(3, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("res/bg.jpg", &width, &height, &nrChannels, 0);
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("res/win.png", &width, &height, &nrChannels, 0);
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("res/loose.png", &width, &height, &nrChannels, 0);
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);
}

void BG::initBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &tbo);

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

	// texture
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
}