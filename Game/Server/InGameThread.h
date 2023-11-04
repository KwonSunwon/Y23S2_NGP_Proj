#pragma once
#include <random>

// 초기 위치
const float initialX[3] = { 0, -0.3, 0.3 };
const float initialY[3] = { 0.3, -0.3, -0.3 };

// 임시 함수
void initClientInfoQueue();

shared_ptr<shared_ptr<LockQueue<Packet>>[]> InitializeInGameThread(BYTE* level);