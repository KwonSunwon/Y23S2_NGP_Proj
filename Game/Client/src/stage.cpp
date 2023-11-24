#pragma once
#include "stage.h"
#include "wall.h"
#include "gameWorld.h"
#include "item.h"
#include "bg.h"

Wall* tempwall;
Item* tempitem;
extern GameWorld gameWorld;

//std::random_device rd4;
std::mt19937 gen4;
std::uniform_int_distribution<int> dis4(0, 360);
float startZ = 18.0f;
void Stage::makePattern(int patternNum)
{
	gen4 = std::mt19937(m_seed);
	std::uniform_int_distribution<int> pattern(0, 10);
	int randomPattern = pattern(gen4);

#ifdef __DEBUG__
	randomPattern = 0;
#endif

	tempwall = new Wall(0);
	// tempwall->initTexture();
	tempitem = new Item(0, 0);
	tempitem->initTexture();

#ifndef LEGACY_PATTERN
	switch (randomPattern)
	{
	case 0:
		// pattern 0 : cross
		for (float i = -0.5; i < 0.5; i += 0.2) {
			tempwall = new Wall(startZ);
			tempwall->setPosX(i);
			tempwall->setPosY(0.f);
			gameWorld.add_object(tempwall);
		}
		for (float i = -0.5; i < 0.5; i += 0.2) {
			tempwall = new Wall(startZ);
			tempwall->setPosX(0.f);
			tempwall->setPosY(i);
			gameWorld.add_object(tempwall);
		}
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	default:
		break;
	}
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