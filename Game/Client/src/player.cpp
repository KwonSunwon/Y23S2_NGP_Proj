#include "player.h"
#include "gameWorld.h"
#include "particle.h"
#include "camera.h"
#include "stage.h"
#include "startStage.h"
#include "Global.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "PacketManager.h"
#include "soundManager.h"

#include "../resource.h"

#ifndef __PLAYER_STATIC__
#define __PLAYER_STATIC__

GLint Player::object = -1;
vector<glm::vec3> Player::vertices;
vector<glm::vec3> Player::normals;
vector<glm::vec2> Player::uvs;

//unsigned int Player::texture = -1;

#endif
extern GameWorld gameWorld;
extern int gameSpeed;
extern SoundManager soundManager;
extern Camera camera;
extern vector<Stage*> stages;
extern int nowStage;
extern StartStage* startStage;

Player::Player()
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
}

void Player::initTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("res/Earth.png", &width, &height, &nrChannels, 0);

	// cout << "check image load" << endl;
	// cout << width << " " << height << " " << nrChannels << endl;

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

void Player::initTexture(const char* path)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "Failed to load texture: " << path << std::endl;
	}

	stbi_image_free(data);
}

void Player::initBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &tbo);

	glBindVertexArray(vao);
	// vertices
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

void Player::render(GLuint shaderProgramID)
{
	if (dieTimer != 0)
		return;
	glUseProgram(shaderProgramID);

	model = glm::mat4(1.0);
	//model = glm::rotate(model, glm::radians(revolution.z), glm::vec3(0, 0, 1));
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.015, 0.015, 0.015));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, Player::object);
}

void Player::colorInit()
{
	for (int i = 0; i < Player::vertices.size(); i++)
	{
		colors.push_back(0.0f);
		colors.push_back(1.0f);
		colors.push_back(0.7f);
	}

	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
}

void Player::update()
{
	die();
	updateItemTimer();
	move();
}

void Player::getEvent(unsigned char key, bool isDown)
{
}

void Player::decideAcc()
{
	setAcc(glm::vec3(0, 0, 0));
	if (isMoveLeft)
	{
		if (isMoveUp || isMoveDown)
			setAccX(-(ACCELERATION / ROOT_TWO));
		else
			setAccX(-ACCELERATION);
	}
	if (isMoveRight)
	{
		if (isMoveUp || isMoveDown)
			setAccX(ACCELERATION / ROOT_TWO);
		else
			setAccX(ACCELERATION);
	}
	if (isMoveUp)
	{
		if (isMoveLeft || isMoveRight)
			setAccY(ACCELERATION / ROOT_TWO);
		else
			setAccY(ACCELERATION);
	}
	if (isMoveDown)
	{
		if (isMoveLeft || isMoveRight)
			setAccY(-(ACCELERATION / ROOT_TWO));
		else
			setAccY(-ACCELERATION);
	}
}

void Player::sendMovePacket() {
	float x = -ACCELERATION * isMoveLeft + ACCELERATION * isMoveRight;
	float y = -ACCELERATION * isMoveDown + ACCELERATION * isMoveUp;
	BYTE flag = 0b10011110;
	flag |= (playerNum << 5);
	g_PacketManager->SendPacket(flag, x, y);
}

void Player::setMoveLeft(bool in)
{
	if (in != isMoveLeft) {
		isMoveLeft = in;
		sendMovePacket();
	}

}
void Player::setMoveRight(bool in)
{
	if (in != isMoveRight) {
		isMoveRight = in;
		sendMovePacket();
	}
}

void Player::setMoveUp(bool in)
{
	if (in != isMoveUp) {
		isMoveUp = in;
		sendMovePacket();
	}

}
void Player::setMoveDown(bool in)
{
	if (in != isMoveDown) {
		isMoveDown = in;
		sendMovePacket();
	}
}

void Player::setPlayerNum(short num)
{
	playerNum = num;
}

void Player::setProtectedMode(bool in)
{
	isProtectedMode = in;
	if (in)
		protectTime = 0;
}
bool Player::getProtectedMode() { return isProtectedMode; }
short Player::getPlayerNum()
{
	return playerNum;
}
void Player::updateItemTimer()
{
	if (isProtectedMode)
	{
		protectTime++;
		if (protectTime == 260)
		{
			isProtectedMode = false;
			protectTime = 0;

			camera.rolling(180.0f, 1);
			camera.setFovy(45.0f);
			camera.setEye(glm::vec3(0, 0, 3.0));
		}
	}
}
void Player::move()
{

	setSpeedX(getSpeed().x + getAcc().x * g_elapsedTime);
	setSpeedY(getSpeed().y + getAcc().y * g_elapsedTime);

	pos.x += speed.x;
	pos.y += speed.y;
	pos.z += speed.z;

	rotate.x += speed.y * 100.f;
	rotate.x -= g_elapsedTime * 100.f;
	rotate.y += speed.x * 100.f;
}

void Player::collision()
{
	float x = numeric_limits<float>::infinity();
	float y = numeric_limits<float>::infinity();
	BYTE flag = 0b00000010;
	flag |= (playerNum << 5);
	g_PacketManager->SendPacket(flag, x, y);

	if (dieTimer != 0)
		return;

	dieTimer++;

	soundManager.soundPlay(PLAYER_DESTROY);

	isAlive = false;
}
void Player::die()
{
	if (dieTimer == 0)
		return;
	dieTimer++;
	if (dieTimer > 100)
	{
		dieTimer = 0;
		pos.x = 10000000;
		pos.y = 10000000;
	}
}
int Player::getPlayerDieTimer()
{
	return dieTimer;
}

bool Player::getIsAlive()
{
	return isAlive;
}

void Player::setIsAlive(bool flag)
{
	isAlive = flag;
}
