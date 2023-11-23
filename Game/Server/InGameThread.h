#pragma once
#include <random>

// EventQueue담을 구조체 아이디어 1
struct EventQueues {
	shared_ptr<LockQueue<Packet>> toServerEventQueue;
	shared_ptr<LockQueue<Packet>> toClientEventQueue;
};

// 플레이어들의 죄표및 가속도 정보를 담을 구조체
struct PlayerPosAcc {
	float PosX = 0;
	float PosY = 0;
	float AccX = 0;
	float AccY = 0;
	float VelX = 0;
	float VelY = 0;
};

// 초기 위치
const float initialX[3] = { 0, -0.3, 0.3 };
const float initialY[3] = { 0.3, -0.3, -0.3 };

// 임시 함수
//void initClientInfoQueue();

//void InitializeInGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER> eventQueues, array<Packet, NUM_OF_PLAYER> playerPackets);
void InGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER> eventQueues);