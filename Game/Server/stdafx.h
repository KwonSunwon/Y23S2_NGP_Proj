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

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

#include "LockQueue.h"
#include "Types.h"

#define NUM_OF_PLAYER 3

static const int NUM_OF_LEVEL = 3;

extern array<LockQueue<ClientInfo>, 3> ClientInfoQueue;

//#define _DEBUG_LOOBY
//#define _DEBUG_CLIENT_SERVER