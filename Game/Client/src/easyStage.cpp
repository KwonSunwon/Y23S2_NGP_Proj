#pragma once
#include "easyStage.h"
#include "player.h"
#include "light.h"
#include "gameWorld.h"
#include "camera.h"
#include "wall.h"
#include "particle.h"
#include "bg.h"
#include "PacketManager.h"

extern Player player;
extern Light light;
extern GameWorld gameWorld;
extern Camera camera;
extern GLuint shaderID;
extern Object *playerPtr;
extern Wall wall;
extern BG backGround;

void EasyStage::init()
{
    cout << "easy Stage" << endl;
     
    backGround.initBuffer();
    backGround.initTexture();
    gameWorld.add_object(&backGround);

    player.initBuffer();
    player.initTexture();
    gameWorld.add_object(playerPtr);

    for(int i= 0 ; i < 2; i++){
        Player* other = new Player();
        other->initBuffer();
        if (i == 0){
            other->setPos(glm::vec3(0.3, 0.3, 0.3));
            other->initTexture("res/Rock.png");
            }
        else{
            other->setPos(glm::vec3(-0.3, 0.3, 0.3));
            other->initTexture("res/Rock.png");
        }
        gameWorld.add_object(other);
        otherPlayers.emplace_back(other);
    }

    makePattern(3);
    for (int i = 0; i < 20; ++i)
    {
        Particle *tempP = new Particle(true);
        tempP->initBuffer();
        gameWorld.add_object(tempP);
    }
}
void EasyStage::update()
{
    gameWorld.update_all();
    light.update(); // ����
    timer++;
    patterTime++;

    // Camera rolling test
    
    Packet* packet;
    while (g_PacketManager->RecvPacket(packet)) {
        short otherIdx = packet->stateMask & 0b01100000;
        float accX = packet->x;
        float accY = packet->y;
        otherPlayers[otherIdx]->setAcc(glm::vec3(accX, accY, 0));
    }

    if (patterTime > 250)
    {
        makePattern(3);
        patterTime = 0;
    }

    glutPostRedisplay();
}
void EasyStage::handleEvent(unsigned char key, bool isDown)
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

        case 's':
            player.setMoveDown(true);
            break;

        case 'w':
            player.setMoveUp(true);
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

        case 'w':
            player.setMoveUp(false);
            break;

        case 'a':
            player.setMoveLeft(false);
            break;

        case 's':
            player.setMoveDown(false);
            break;

        case 'd':
            player.setMoveRight(false);
            break;
        }
    }
}
void EasyStage::draw()
{
    camera.setCamera(shaderID, 0); // 0 = �������� / 1 = ��������
    light.setLight(shaderID, camera.getEye());
    gameWorld.draw_all();
}
void EasyStage::out()
{
    g_PacketManager->Reset();
    patterTime = 0;
    cout << "Out Easy Stage" << endl;
}