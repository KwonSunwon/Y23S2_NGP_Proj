#pragma once
#include "gameManager.h"
#include "player.h"
#include "gameWorld.h"
#include "wall.h"
#include "stage.h"
#include "mainStage.h"
#include "startStage.h"
#include "easyStage.h"
#include <vector>
extern GameWorld gameWorld;
extern vector<Stage*> stages;
extern int nowStage;
MainStage* mainStage = new MainStage();
StartStage* startStage = new StartStage();
EasyStage* easyStage = new EasyStage();
GameManager::GameManager()
{
}

void GameManager::gameRun()
{
	//stages.push_back(startStage);
	stages.push_back(easyStage);
	cout << ("gameRun");
	stages[nowStage]->init();
}

void GameManager::handleEvent(unsigned char key, bool isDown)
{

	stages[nowStage]->handleEvent(key, isDown);
}

void GameManager::handleEvent(int button, int state, int x, int y)
{

	stages[nowStage]->handleEvent(button, state, x, y);
}

void GameManager::update()
{
	stages[nowStage]->update();
}
void GameManager::draw()
{
	stages[nowStage]->draw();
}


