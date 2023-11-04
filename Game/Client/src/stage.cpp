#pragma once
#include "stage.h"
#include "wall.h"
#include "gameWorld.h"
#include "item.h"
#include "bg.h"

Wall *tempwall;
Item *tempitem;
extern GameWorld gameWorld;

std::random_device rd4;
std::mt19937 gen4(rd4());
std::uniform_int_distribution<int> dis4(0, 360);
float startZ = 18.0f;
void Stage::makePattern(int patternNum)
{
    tempwall = new Wall(0, 0);
    tempwall->initTexture();
    tempitem = new Item(0, 0);
    tempitem->initTexture();
    switch (patternNum)
    {
    // ï¿½ï¿½ï¿½Ú¸ï¿½ï¿?
    case 0:
        cout << "make pattern case:" << patternNum << endl;
        for (int i = 0; i < 9; ++i)
        {

            tempwall = new Wall(startZ, 40.0f * i);
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);

            tempwall = new Wall(startZ + 2.0f, 40.0f * i + 15.0f);
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
            tempwall = new Wall(startZ + 4.0f, 40.0f * i);
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
        }
        break;
    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?
    case 1:
        cout << "make pattern case:" << patternNum << endl;
        for (int i = 0; i < 24; ++i)
        {
            if (i == 0)
            {
                tempitem = new Item(startZ + (float)i * 0.3, 15.0f * (i + 12 % 24));
                tempitem->initBuffer();
                gameWorld.add_object(tempitem);
                continue;
            }

            tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * (i + 12 % 24));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
        }
        break;

    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    case 2:
        cout << "make pattern case:" << patternNum << endl;
        for (int i = 0; i < 24; ++i)
        {
            if (i == 0)
            {
                tempitem = new Item(startZ + (float)i * 0.3, 15.0f * (i % 24));
                tempitem->initBuffer();
                gameWorld.add_object(tempitem);
                continue;
            }

            tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * (i % 24));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);

            tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((i + 12) % 24));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
        }
        break;
    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?
    case 3:
        cout << "make pattern case:" << patternNum << endl;
        for (int i = 0; i < 24; ++i)
        {

            tempwall = new Wall(startZ + (float)i * 0.3, (float)dis4(gen4));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
        }
        break;
    // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
    case 4:
        cout << "make pattern case:" << patternNum << endl;
        for (int i = 0; i < 12; ++i)
        {
            if (i == 0)
            {
                tempitem = new Item(startZ + (float)i * 0.3, 15.0f * (i % 24));
                tempitem->initBuffer();
                gameWorld.add_object(tempitem);
                continue;
            }

            tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * (i % 24));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);

            tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((i + 12) % 24));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
        }

        for (int i = 12; i < 24; ++i)
        {

            tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((24 - i) % 24));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);

            tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((36 - i) % 24));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
        }
        break;
    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    case 100:
        cout << "make pattern case:" << patternNum << endl;
        for (int i = 0; i < 24; ++i)
        {

            tempwall = new Wall(startZ - 5.0f + (float)i * 0.3, (float)dis4(gen4));
            tempwall->initBuffer();
            gameWorld.add_object(tempwall);
        }
        break;
    default:
        cout << patternNum << "default" << endl;
    }
}