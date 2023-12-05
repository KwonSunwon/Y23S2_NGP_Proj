#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "object.h"
#include "stdafx.h"

class Player : public Object
{
private:
	int dieTimer = 0;
	static GLint object;
	static vector<glm::vec3> vertices;
	static vector<glm::vec3> normals;
	static vector<glm::vec2> uvs;

	unsigned int texture;

protected:
	bool isMoveLeft = false;
	bool isMoveRight = false;
	bool isMoveUp = false;
	bool isMoveDown = false;
	bool isProtectedMode = false;
	short playerNum = 0;
	int protectTime = 0;
	bool isAlive = true;

public:
	Player();
	void render(GLuint shaderProgramID) override;
	void colorInit();
	void initTexture() override;
	void initBuffer() override;
	void initTexture(const char* fileName) override;

	void update() override;
	void getEvent(unsigned char key, bool isDown);
	void decideAcc();
	void setMoveLeft(bool);
	void setMoveRight(bool);
	void setMoveUp(bool);
	void setMoveDown(bool);
	void setPlayerNum(short);
	void setProtectedMode(bool);
	bool getProtectedMode();
	short getPlayerNum();
	void updateItemTimer();
	void collision();
	void move();
	void die();
	int getPlayerDieTimer();

	bool getIsAlive();
	void setIsAlive(bool);
};

#endif
