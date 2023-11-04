#pragma once
#include "normalStage.h"
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

std::random_device rdStage1;
std::mt19937 genStage1(rdStage1());
std::uniform_int_distribution<int> disStage1(0, 4);

void NormalStage::init()
{
    cout << "normal Stage" << endl;

    backGround.initBuffer();
    backGround.initTexture();
    gameWorld.add_object(&backGround);  

    player.initBuffer();
    player.initTexture();
    gameWorld.add_object(playerPtr);

    /*for (int i = 0; i < 100; ++i)
    {
        cout << i << endl;
        Wall *tempwall = new Wall();
        tempwall->initBuffer();
        tempwall->initTexture();
        gameWorld.add_object(tempwall);
    }*/
    makePattern(1);
    for (int i = 0; i < 20; ++i)
    {
        cout << i << endl;
        Particle* tempP = new Particle(true);
        tempP->initBuffer();
        gameWorld.add_object(tempP);
    }
}
void NormalStage::update()
{
    gameWorld.update_all();
    light.update(); // ����
    timer++;
    patterTime++;


    camera.rolling(1.0f, -1); // angle, direction
 

    if (patterTime > 250)
    {
        patterTime = 0;
        makePattern(disStage1(genStage1));
    }

    glutPostRedisplay();
}
void NormalStage::handleEvent(unsigned char key, bool isDown)
{
    if (isDown)
    {
        switch (key)
        {
        case'c':
            makePattern(1);
            //player.setProtectedMode(true);
            break;
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
void NormalStage::draw()
{
    camera.setCamera(shaderID, 0); // 0 = �������� / 1 = ��������
    light.setLight(shaderID, camera.getEye());
    gameWorld.draw_all();
}
void NormalStage::out()
{
}