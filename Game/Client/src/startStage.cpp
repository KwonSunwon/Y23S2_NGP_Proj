#pragma once
#include "startStage.h"
#include "player.h"
#include "light.h"
#include "gameWorld.h"
#include "camera.h"
#include "wall.h"
#include "easyStage.h"
#include "normalStage.h"
#include "hardStage.h"
#include "menu.h"
#include "soundManager.h"
#include "PacketManager.h"

extern Player player;
extern Light light;
extern GameWorld gameWorld;
extern Camera camera;
extern GLuint shaderID;
extern Object *playerPtr;
extern vector<Stage *> stages;
extern int nowStage;
extern SoundManager soundManager;

PacketManager* g_packetManager;
EasyStage *easyStage = new EasyStage();
NormalStage *normalStage = new NormalStage();
HardStage *hardStage = new HardStage();

Menu *mainMenu;

void StartStage::init()
{
    cout << "StartStage" << endl;
    cout << "Loading..." << endl;
    Wall *wall = new Wall(0, 0);
    wall->initBuffer();
    wall->initTexture();
    delete wall;
    cout << "Complete!" << endl;

    mainMenu = new Menu();
    mainMenu->initBuffer();
    mainMenu->initTexture();
    if (!soundManager.getIsBgmPlay())
    {
        soundManager.setIsBgmPlay(true);
        soundManager.soundPlay(BGM);
    }
    light.setAmbientLight(1.0);
}

void StartStage::update()
{
    glutPostRedisplay();
}

void StartStage::handleEvent(unsigned char key, bool isDown)
{
    if (isDown)
    {
        switch (key)
        {
        case '1':
            stages.back()->out();
            g_packetManager = new PacketManager();
            g_packetManager->Initialize(GAME_LEVEL::EASY);
            nowStage++;
            stages.push_back(easyStage);
            stages[nowStage]->init();
            break;
        case '2':
            stages.back()->out();
            g_packetManager = new PacketManager();
            g_packetManager->Initialize(GAME_LEVEL::NORMAL);
            nowStage++;
            stages.push_back(normalStage);
            stages[nowStage]->init();
            break;
        case '3':
            stages.back()->out();
            g_packetManager = new PacketManager();
            g_packetManager->Initialize(GAME_LEVEL::HARD);
            nowStage++;
            stages.push_back(hardStage);
            stages[nowStage]->init();
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
        }
    }
}

void StartStage::handleEvent(int button, int state, int x, int y)
{
    int botton = mainMenu->isClicked(x, y);
    switch (botton)
    {
    case 1:
        stages.back()->out();
        //g_packetManager = new PacketManager(GAME_LEVEL::EASY);
        nowStage++;
        stages.push_back(easyStage);
        stages[nowStage]->init();
        break;
    case 2:
        stages.back()->out();
        nowStage++;
        stages.push_back(normalStage);
        stages[nowStage]->init();
        break;
    case 3:
        stages.back()->out();
        nowStage++;
        stages.push_back(hardStage);
        stages[nowStage]->init();
        break;
    case 4:
        exit(0);
        break;
    }
}

void StartStage::draw()
{
    camera.setCamera(shaderID, 0);
    light.setLight(shaderID, camera.getEye());
    mainMenu->render(shaderID);
    gameWorld.draw_all();
}

void StartStage::out()
{
    light.setAmbientLight(0.5);
}