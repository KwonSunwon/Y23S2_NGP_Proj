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
	model = glm::rotate(model, glm::radians(revolution.z), glm::vec3(0, 0, 1));
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

void Player::setMoveLeft(bool in)
{
	if (in != isMoveLeft) {
		isMoveLeft = in;
		{
			float x = -0.005 * isMoveLeft + 0.005 * isMoveRight;
			float y = -0.005 * isMoveDown + 0.005 * isMoveUp;
			BYTE flag = 0b10011110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);
		}
		{
			/*float x = pos.x;
			float y = pos.y;
			BYTE flag = 0b10001110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);*/
		}	
	}
	
}
void Player::setMoveRight(bool in)
{
	if (in != isMoveRight) {
		isMoveRight = in;
		{
			float x = -0.005 * isMoveLeft + 0.005 * isMoveRight;
			float y = -0.005 * isMoveDown + 0.005 * isMoveUp;
			BYTE flag = 0b10011110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);
		}
		{
		/*	float x = pos.x;
			float y = pos.y;
			BYTE flag = 0b10001110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);*/
		}
	}
}
	
void Player::setMoveUp(bool in)
{
	if (in != isMoveUp) {
		isMoveUp = in;
		{
			float x = -0.005 * isMoveLeft + 0.005 * isMoveRight;
			float y = -0.005 * isMoveDown + 0.005 * isMoveUp;
			BYTE flag = 0b10011110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);
		}
		{
			/*float x = pos.x;
			float y = pos.y;
			BYTE flag = 0b10001110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);*/
		}
	}
	
}
void Player::setMoveDown(bool in)
{
	if (in != isMoveDown) {
		isMoveDown = in;
		{
			float x = -0.005 * isMoveLeft + 0.005 * isMoveRight;
			float y = -0.005 * isMoveDown + 0.005 * isMoveUp;
			BYTE flag = 0b10011110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);
		}
		{
			/*float x = pos.x;
			float y = pos.y;
			BYTE flag = 0b10001110;
			flag |= (playerNum << 5);
			g_PacketManager->SendPacket(flag, x, y);*/
		}
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


	//if (isMoveLeft)
	//{
	//	setSpeedX(getSpeed().x - 0.002);
	//}
	//if (isMoveRight)
	//{
	//	setSpeedX(getSpeed().x + 0.002);
	//}
	//if (isMoveUp)
	//{
	//	setSpeedY(getSpeed().y + 0.002);
	//}
	//if (isMoveDown)
	//{
	//	setSpeedY(getSpeed().y - 0.002);
	//}

	pos.x += speed.x;
	pos.y += speed.y;
	pos.z += speed.z;

	if (revolution.z < 0)
		revolution.z += 360.0f;
	if (revolution.z > 360)
		revolution.z -= 360.0f;
}
void Player::collision()
{
	if (dieTimer != 0)
		return;
	for (int i = 0; i < 500; ++i)
	{
		Particle* tempP = new Particle(false);
		tempP->initBuffer();
		gameWorld.add_object(tempP);
	}

	dieTimer++;

	soundManager.soundPlay(PLAYER_DESTROY);
}
void Player::die()
{
	if (dieTimer == 0)
		return;
	dieTimer++;
	if (dieTimer > 100)
	{
		//soundManager.soundStop(0);
		dieTimer = 0;
		pos.x = 10000000;
		pos.y = 10000000;
		/*camera.setRoll(0);
		gameWorld.del_objects();
		stages.back()->out();
		nowStage++;
		stages.push_back(startStage);
		stages[nowStage]->init();*/
	}
}
int Player::getPlayerDieTimer()
{
	return dieTimer;
}
