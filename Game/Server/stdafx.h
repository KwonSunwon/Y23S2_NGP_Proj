#pragma once

#include <array>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <limits>

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

#include <windows.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
// stateMask 체크를 위해 include
#include <bitset>
using namespace std;

#include "LockQueue.h"
#include "Types.h"


#define MAX_SPEED 0.04f

#define COEF 0.00008f
#define ROOT_TWO 1.41421f

#define NUM_OF_PLAYER 3
#define END_OF_X 1.27f
#define END_OF_Y 1.27f

#define PLAYER_RADIUS 0.12f
#define PLAYER_MASS 0.4f

static const int NUM_OF_LEVEL = 3;

extern array<LockQueue<ClientInfo>, 3> ClientInfoQueue;

//#define _DEBUG_LOOBY
//#define _DEBUG_CLIENT_SERVER
//#define _DEBUG_INGAME