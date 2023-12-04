#pragma once
#include "hardStage.h"
#include "player.h"
#include "light.h"
#include "gameWorld.h"
#include "camera.h"
#include "wall.h"
#include "particle.h"
#include "bg.h"

extern Player player;
extern Light light;
extern GameWorld gameWorld;
extern Camera camera;
extern GLuint shaderID;
extern Object* playerPtr;
extern Wall wall;
extern BG backGround;

std::random_device rdStage;
std::mt19937 genStage(rdStage());
std::uniform_int_distribution<int> disStage(0, 4);
void HardStage::init()
{
	cout << "hard Stage" << endl;

	backGround.initBuffer();
	backGround.initTexture();
	gameWorld.add_object(&backGround);

	player.initBuffer();
	player.initTexture();
	gameWorld.add_object(playerPtr);
	makePattern(100);
	makePattern(0);
	for (int i = 0; i < 20; ++i)
	{
		Particle* tempP = new Particle(true);
		tempP->initBuffer();
		gameWorld.add_object(tempP);
	}
}
void HardStage::update()
{
	gameWorld.update_all();
	light.update(); // ����
	timer++;
	patternTime++;

	// Camera rolling test
	if (timer % 300 < 150)
		camera.rolling((timer % 150 * 0.01 + 0.4f), -1); // angle, direction
	else
		camera.rolling((timer % 150 * 0.01 + 0.4f), 1);

	if (patternTime > 250)
	{
		patternTime = 0;
		makePattern(disStage(genStage));
	}

	glutPostRedisplay();
}

void HardStage::handleEvent(unsigned char key, bool isDown)
{

	if (isDown)
	{
		switch (key)
		{
		case 'a':
			player.setMoveLeft(true);
			break;
		case 'd':
			player.setMoveRight(true);
			break;
		case 'Q':
		case 'q':
			exit(0);
			break;
		}
	}
	else if (!isDown)
	{
		switch (key)
		{

		case 'a':
			player.setMoveLeft(false);
			break;
		case 'd':
			player.setMoveRight(false);
			break;
		}
	}
}
void HardStage::draw()
{
	camera.setCamera(shaderID, 0); // 0 = �������� / 1 = ��������
	light.setLight(shaderID, camera.getEye());
	gameWorld.draw_all();
}
void HardStage::out()
{
	patternTime = 0;
}