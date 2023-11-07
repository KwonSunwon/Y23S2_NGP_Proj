#include "stdafx.h"
#include "InGameThread.h"

// 랜덤 시드를 위한 랜덤 엔진
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 15);

// 임시 ClientInfoQueue

//void initClientInfoQueue()
//{
//	ClientInfo a[3];
//	for (int i = 0; i < 3; ++i) {
//		shared_ptr<LockQueue<Packet>> p = make_shared<LockQueue<Packet>>();
//		a[i].level = GAME_LEVEL::EASY;
//		a[i].packetQueptr = p;
//		a[i].sock = 0;
//		ClientInfoQueue.Push(a[i]);
//	}
//}
// ---------------------------------------

// 쓰레드 함수내 쓰레드 초기화 함수
// 아이디어 1 구조체 이용
bool InitializeInGameThread(GAME_LEVEL* level, EventQueues eventQueues[NUM_OF_PLAYER])
{
	ClientInfo clientInfo;
	Packet pack;
	int seed = dis(gen);

	for (int i = 0; i < NUM_OF_PLAYER; ++i) {
		// ClientInfoQueue에서 Packet 데이터 pop
		if (!ClientInfoQueue.TryPop(clientInfo)) return false;
		// stateMask 초기화
		pack.stateMask = 0;
		// 시작 상태
		pack.stateMask |= (1 << (int)STATE_MASK::GAME_START);
		// 플레이어 번호
		pack.stateMask |= (i << (int)STATE_MASK::PLAYER_NUM);
		// 초기위치설정, opengl로 좌표변환은 Client에서
		pack.stateMask &= ~(1 << (int)STATE_MASK::POS_FLAG);
		pack.x = initialX[i];
		pack.y = initialY[i];
		// 랜덤 시드 값
		pack.stateMask |= seed;
		// toClientEventQueue
		clientInfo.toClientEventQueue = eventQueues[i].toClientEventQueue;
		clientInfo.toServerEventQueue = eventQueues[i].toServerEventQueue;
		eventQueues[i].toClientEventQueue->Push(pack);
		
	}
	// 난이도 어떻게 전달?
	*level = clientInfo.level;
}


// 아이디어2 구조체 없이 주소값만 가지고 할당
//void InitializeInGameThread(GAME_LEVEL* level, shared_ptr<shared_ptr<LockQueue<Packet>>[]> toSeverEventQueue,  
//	shared_ptr<shared_ptr<LockQueue<Packet>>[]> toClientEventQueue)


//void InGameThread(GAME_LEVEL *level)
//{
//	bool timeReset = false;
//	chrono::system_clock::time_point start;
//	chrono::duration<double> time;
//	EventQueues eventQueues[NUM_OF_PLAYER];
//	InitializeInGameThread(level, eventQueues);
//
//	while (true) {
//		if (numOfPlayer == 0)
//			break;
//		if (numOfPlater == 1) {
//			PushWinPacket();
//			break;
//		}
//		// 물리클래스로 물리검사
//		Physics.Caculate(eventQueues)
//		// 결과 업데이트
//		// 충돌 방생시 출돌정보 push [0__1__10]
//		PushPacket(eventQueues);
//		// 그에 따른 가속도 push
//		PushPacket(eventQueues);
//
//		if (timeReset == false) {
//			timeReset = true;
//			start = chrono::system_clock::now();
//		}
//		time = chrono::system_clock::now() - start;
//		// 위치 보정
//		if (time.count() >= 0.3) {
//			// 위치 push [0__0__10]
//			PushPacket(eventQueues);
//			timeReset == false;
//		}
//	}
//}