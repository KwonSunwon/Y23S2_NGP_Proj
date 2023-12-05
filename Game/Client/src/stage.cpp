#pragma once
#include "stage.h"
#include "wall.h"
#include "gameWorld.h"
#include "item.h"
#include "bg.h"

extern GameWorld gameWorld;

float startZ = -18.0f;
void Stage::makePattern(int patternNum)
{
	float patternPosX;
	float patternPosY;

	const unsigned short indexSize = 120;
#ifndef LEGACY_PATTERN
	switch (patternNum)
	{
	case 0:
		// pattern 0: cross +
		patternPosX = -0.8f;
		for (int i = 0; i < 10; i++) {
			//tempwall = new Wall(startZ);
			//tempwall->setPosX(i);
			//tempwall->setPosY(0.f);
			//gameWorld.add_object(tempwall);
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(patternPosX);
			patternPosX += 0.2f;
			walls[wallIndex]->setPosY(0.f);
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);

		}
		patternPosY = -0.8f;
		for (int i = 0; i < 10; i++) {
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(0.f);
			walls[wallIndex]->setPosY(patternPosY);
			patternPosY += 0.2f;
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);
		}
		break;
	case 1:
		// pattern 1: cross x
		patternPosX = -0.8f;
		patternPosY = -0.8f;
		for (int i = 0; i < 10; i++) {
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(patternPosX);
			walls[wallIndex]->setPosY(patternPosY);
			patternPosX += 0.2f;
			patternPosY += 0.2f;
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);
		}
		patternPosX = -0.8f;
		patternPosY = -0.8f;
		for (int i = 0; i < 10; i++) {
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(patternPosX);
			walls[wallIndex]->setPosY(-patternPosY);
			patternPosX += 0.2f;
			patternPosY += 0.2f;
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);
		}
		break;
	case 2:
		// pattern 2: rectangle ¤±
		patternPosX = -0.8f;
		for (int i = 0; i < 5; i++) {
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(patternPosX);
			patternPosX += 0.4f;
			walls[wallIndex]->setPosY(0.8);
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);
		}
		patternPosX = -0.8f;
		for (int i = 0; i < 5; i++) {
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(patternPosX);
			patternPosX += 0.4f;
			walls[wallIndex]->setPosY(-0.8);
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);
		}
		patternPosY = -0.8f;
		for (int i = 0; i < 5; i++) {
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(0.8);
			walls[wallIndex]->setPosY(patternPosY);
			patternPosY += 0.4f;
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);
		}
		patternPosY = -0.8f;
		for (int i = 0; i < 5; i++) {
			wallIndex = ++wallIndex % indexSize;
			walls[wallIndex]->setPosZ(startZ);
			walls[wallIndex]->setPosX(-0.8);
			walls[wallIndex]->setPosY(patternPosY);
			patternPosY += 0.4f;
			static_cast<Wall*>(walls[wallIndex])->setAlive(true);
		}
		break;
	case 3:
		// pattern 3: window half(upside)
		patternPosX = -0.8f;
		patternPosY = 0.8f;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				wallIndex = ++wallIndex % indexSize;
				walls[wallIndex]->setPosZ(startZ);
				walls[wallIndex]->setPosX(patternPosX);
				walls[wallIndex]->setPosY(patternPosY);
				patternPosX += 0.4f;
				static_cast<Wall*>(walls[wallIndex])->setAlive(true);
			}
			patternPosX = -0.8f;
			patternPosY -= 0.2f;
		}
		break;
	case 4:
		// pattern 4: window half(downside)
		patternPosX = -0.8f;
		patternPosY = -0.8f;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				wallIndex = ++wallIndex % indexSize;
				walls[wallIndex]->setPosZ(startZ);
				walls[wallIndex]->setPosX(patternPosX);
				walls[wallIndex]->setPosY(patternPosY);
				patternPosX += 0.4f;
				static_cast<Wall*>(walls[wallIndex])->setAlive(true);
			}
			patternPosX = -0.8f;
			patternPosY += 0.2f;
		}
		break;
	case 5:
		// pattern 7: sprite
		patternPosX = -0.4f;
		patternPosY = -0.8f;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				wallIndex = ++wallIndex % indexSize;
				walls[wallIndex]->setPosZ(startZ);
				walls[wallIndex]->setPosX(patternPosX);
				walls[wallIndex]->setPosY(patternPosY);
				patternPosY += 0.2f;
				static_cast<Wall*>(walls[wallIndex])->setAlive(true);
			}
			patternPosX = 0.4f;
			patternPosY = -0.8f;
		}
		break;
	case 6:
		// pattern 8: sprite 2
		patternPosX = -0.8f;
		patternPosY = -0.4f;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				wallIndex = ++wallIndex % indexSize;
				walls[wallIndex]->setPosZ(startZ);
				walls[wallIndex]->setPosX(patternPosX);
				walls[wallIndex]->setPosY(patternPosY);
				patternPosX += 0.2f;
				static_cast<Wall*>(walls[wallIndex])->setAlive(true);
			}
			patternPosX = -0.8f;
			patternPosY = 0.4f;
		}
		break;
	default:
		break;
	}
	cout << "make pattern case:" << patternNum << endl;
#endif // !LEGACY_PATTERN

#ifdef LEGACY_PATTERN
	switch (patternNum)
	{
	case 0:
		cout << "make pattern case:" << patternNum << endl;
		for (int i = 0; i < 9; ++i)
		{

			tempwall = new Wall(startZ, 40.0f * i);
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);

			tempwall = new Wall(startZ + 2.0f, 40.0f * i + 15.0f);
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
			tempwall = new Wall(startZ + 4.0f, 40.0f * i);
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
		}
		break;
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
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
		}
		break;
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
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);

			tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((i + 12) % 24));
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
		}
		break;
	case 3:
		cout << "make pattern case:" << patternNum << endl;
		for (int i = 0; i < 24; ++i)
		{

			tempwall = new Wall(startZ + (float)i * 0.3, (float)dis4(gen4));
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
		}
		break;
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
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);

			tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((i + 12) % 24));
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
		}

		for (int i = 12; i < 24; ++i)
		{

			tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((24 - i) % 24));
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);

			tempwall = new Wall(startZ + (float)i * 0.3, 15.0f * ((36 - i) % 24));
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
		}
		break;
	case 100:
		cout << "make pattern case:" << patternNum << endl;
		for (int i = 0; i < 24; ++i)
		{

			tempwall = new Wall(startZ - 5.0f + (float)i * 0.3, (float)dis4(gen4));
			// tempwall->initBuffer();
			gameWorld.add_object(tempwall);
		}
		break;
	default:
		cout << patternNum << "default" << endl;
	}
#endif // LEGACY_PATTERN
}