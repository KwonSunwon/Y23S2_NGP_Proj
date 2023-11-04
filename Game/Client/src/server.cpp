#pragma once
#include "gameWorld.h"
#include "light.h"
#include "camera.h"
#include "wall.h"
#include "player.h"
#include "gameManager.h"
#include "object.h"
#include "stage.h"
#include "soundManager.h"
#include "bg.h"
#include <vector>

extern GLint g_width;
extern GLint g_height;
int idNum = 0;
GameWorld gameWorld;
Light light;
Camera camera(g_width, g_height);
Player player;
GameManager gameManager;
Object *playerPtr = &player;
int nowStage = 0;
vector<Stage *> stages;
int gameSpeed = 60;
SoundManager soundManager;
BG backGround;