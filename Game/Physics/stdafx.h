#pragma once
#ifndef __STDAFX_H__
#define __STDAFX_H__
#define _CRT_SECURE_NO_WARNINGS

#include "../Client/api/OpenGL/glew.h"
#include "../Client/api/OpenGL/freeglut.h"
#include "../Client/api/OpenGL/freeglut_ext.h"
#include "../Client/api/OpenGL/glm/glm.hpp"
#include "../Client/api/OpenGL/glm/ext.hpp"
#include "../Client/api/OpenGL/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_SPEED 0.04f
#define ACCELERATION 0.01f
#define COEF 0.003f
#define ROOT_TWO 1.41421f

#define NUM_OF_PLAYER 3
#define END_OF_X 1.06f
#define END_OF_Y 0.8f

using namespace std;

#endif