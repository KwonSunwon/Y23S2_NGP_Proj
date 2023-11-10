#pragma once
#include <random>

// EventQueue담을 구조체 아이디어 1
struct EventQueues {
	shared_ptr<LockQueue<Packet>> toServerEventQueue;
	shared_ptr<LockQueue<Packet>> toClientEventQueue;
};

// 초기 위치
const float initialX[3] = { 0, -0.3, 0.3 };
const float initialY[3] = { 0.3, -0.3, -0.3 };

// 임시 함수
//void initClientInfoQueue();

void InitializeInGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER> eventQueues, array<Packet, NUM_OF_PLAYER> playerPackets);
void InGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER> eventQueues);