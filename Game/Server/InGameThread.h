#pragma once
#include <random>
#include "CollisionManager.h"

// EventQueue담을 구조체 아이디어 1
struct EventQueues {
	shared_ptr<LockQueue<Packet>> toServerEventQueue;
	shared_ptr<LockQueue<Packet>> toClientEventQueue;
};

// 초기 위치
const float initialX[3] = { 0.f, -0.3f, 0.3f };
const float initialY[3] = { 0.3f, -0.3f, -0.3f };

// 임시 함수
//void initClientInfoQueue();

void InGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER> eventQueues);