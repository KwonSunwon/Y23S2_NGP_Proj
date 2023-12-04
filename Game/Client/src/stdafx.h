#ifndef __STDAFX_H__
#define __STDAFX_H__

#define BGM 0
#define BUTTON_CLICK 1
#define PLAYER_DESTROY 2
#define WALL_DESTROY 3
#define ITEM_DESTROY 4

enum class STATE_MASK {
	RESULT,
	PLAYING,
	LIFE,
	POS_FLAG,
	PLAYER_NUM,
	GAME_START
};
#define MAX_SPEED 0.04f
#define ACCELERATION 0.01f
#define COEF 0.003f
#define ROOT_TWO 1.41421f

#define __DEBUG__

#if FREEGLUT_LIB_PRAGMAS
#	ifdef NDEBUG
#		pragma comment(lib, "freeglut.lib")
#	else
#		pragma comment(lib, "freeglutd.lib")
#	endif
#endif

#pragma warning(disable : 4996)

// OpenGL
//#include <gl/glew.h>
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <gl/glm/glm.hpp>
//#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>
#include "../api/OpenGL/glew.h"
#include "../api/OpenGL/freeglut.h"
#include "../api/OpenGL/freeglut_ext.h"
#include "../api/OpenGL/glm/glm.hpp"
#include "../api/OpenGL/glm/ext.hpp"
#include "../api/OpenGL/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <queue>
#include <memory>
#include <chrono>
#include <ratio>
#include <array>
#include "Global.h"

#include <CommCtrl.h>
#include <thread>

using namespace std;

#define ACCELERATION 0.01f

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

#include <fmod.hpp>

#endif