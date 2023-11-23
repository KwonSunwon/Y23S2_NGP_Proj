
#include "stdafx.h"
#include "InGameThread.h"

// 랜덤 시드를 위한 랜덤 엔진
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 15);


static void ToServerQueueCheck(vector<int> alivePlayer, array<EventQueues, NUM_OF_PLAYER>* eventQueues, array<Packet, NUM_OF_PLAYER>* playerPackets)
{
	for (auto player : alivePlayer)
	{
		(*eventQueues)[player].toServerEventQueue->TryPop((*playerPackets)[player]);
	}
}

static void PushWinPacket(EventQueues winnerQueues)
{
	Packet winPack;
	winnerQueues.toServerEventQueue->WaitPop(winPack);
	winPack.stateMask |= (1 << (int)STATE_MASK::RESULT);
	winnerQueues.toClientEventQueue->Push(winPack);
}

static void PushPacket(vector<int> alivePlayer, array<EventQueues, NUM_OF_PLAYER>* eventQueues, array<Packet, NUM_OF_PLAYER>* playerPackets)
{
	for (auto player : alivePlayer)
	{
		for (int i = 0; i < NUM_OF_PLAYER; ++i)
			(*eventQueues)[player].toClientEventQueue->Push((*playerPackets)[i]);
	}
}

static void PrintPacketData(array<Packet, NUM_OF_PLAYER> playerPackets)
{
	int num = 1;
	for (auto packet : playerPackets)
	{
		cout << "[PLAYER_" << num << "] stateMask: ";
		cout << bitset<8>(packet.stateMask);
		cout << endl;
		cout << "[PLAYER_" << num << "] x, y: " << packet.x << ", " << packet.y << endl;
		++num;
	}
}

// 쓰레드 함수내 쓰레드 초기화 함수
void InitializeInGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER>* eventQueues, array<Packet, NUM_OF_PLAYER> *playerPackets, array<PlayerPosAcc, NUM_OF_PLAYER>* players)
{
	//ClientInfo clientInfo;
	int seed = dis(gen);

	for (int i = 0; i < NUM_OF_PLAYER; ++i) {
		// ClientInfoQueue에서 Packet 데이터 pop
		//ClientInfoQueue[(int)level].WaitPop(clientInfo);
		// stateMask 초기화
		(*playerPackets)[i].stateMask = 0;
		// 시작 상태
		(*playerPackets)[i].stateMask |= (1 << (int)STATE_MASK::GAME_START);
		// 플레이어 번호
		(*playerPackets)[i].stateMask |= (i << (int)STATE_MASK::PLAYER_NUM);
		// 초기위치설정, opengl로 좌표변환은 Client에서
		(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::POS_FLAG);
		(*playerPackets)[i].x = initialX[i];
		(*playerPackets)[i].y = initialY[i];
		(*players)[i].PosX = initialX[i];
		(*players)[i].PosY = initialY[i];
		// 랜덤 시드 값
		(*playerPackets)[i].stateMask |= seed;
		// toClientEventQueue
		//clientInfo.toClientEventQueue = eventQueues[i].toClientEventQueue;
		//clientInfo.toServerEventQueue = eventQueues[i].toServerEventQueue;
		for (int j = 0; j < NUM_OF_PLAYER ; ++j)
			(*eventQueues)[i].toClientEventQueue->Push((*playerPackets)[j]);
	}
}

void InitPacket(array<Packet, NUM_OF_PLAYER>* playerPackets)
{
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
	{
		(*playerPackets)[i].x = 0;
		(*playerPackets)[i].y = 0;
		(*playerPackets)[i].stateMask |= (1 << (int)STATE_MASK::GAME_START);
		(*playerPackets)[i].stateMask |= (1 << (int)STATE_MASK::POS_FLAG);
		(*playerPackets)[i].stateMask |= (3 << (int)STATE_MASK::LIFE);
		(*playerPackets)[i].stateMask |= (1 << (int)STATE_MASK::PLAYING);
		(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::RESULT);
	}
}

void CaculateAcceleration(vector<int> alivePlayer, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerPosAcc, NUM_OF_PLAYER>* players)
{
	for (auto player : alivePlayer)
	{
		(*players)[player].AccX = (*playerPackets)[player].x;
		(*players)[player].VelX += (*players)[player].AccX;
		(*players)[player].PosX += (*players)[player].VelX;
		(*players)[player].AccY = (*playerPackets)[player].y;
		(*players)[player].VelY += (*players)[player].AccY;
		(*players)[player].PosY += (*players)[player].VelY;
	}
}

void InGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER> eventQueues)
{
#ifdef _DEBUG_INGAME
	cout << "InGame Thread Start" << endl;
#endif // _DEBUG_INGAME

	bool timeReset = false;
	chrono::system_clock::time_point start;
	chrono::duration<double> time;

	array<Packet, NUM_OF_PLAYER> playerPackets;
	array<PlayerPosAcc, NUM_OF_PLAYER> players;
	memset(&playerPackets, 0, sizeof(Packet) * NUM_OF_PLAYER);

	// 플레이어가 죽으면 erase(player_num);
	vector<int>alivePlayer(NUM_OF_PLAYER);
	iota(alivePlayer.begin(), alivePlayer.end(), 0);

	InitializeInGameThread(level, &eventQueues, &playerPackets, &players);
#ifdef _DEBUG_INGAME
	cout << "패킷데이터 확인" << endl;
	PrintPacketData(playerPackets);
#endif // _DEBUG_INGAME
	InitPacket(&playerPackets);
	while (true) {
		if (alivePlayer.size() == 0)
			break;
		if (alivePlayer.size() == 1) {
			PushWinPacket(eventQueues[alivePlayer.front()]);
			alivePlayer.clear();
			break;
		}
		ToServerQueueCheck(alivePlayer, &eventQueues, &playerPackets);
		// 물리클래스로 물리검사
		//Physics.Caculate(eventQueues)
		// 결과 업데이트
		// 충돌 방생시 출돌정보 push [0__1__10]
		// 플레이어 사망시 erase(player_num);
		// 1명만 남을 시 winPlayer에 기록
		//PushPacket(eventQueues);
		// 그에 따른 가속도 push
		PushPacket(alivePlayer, &eventQueues, &playerPackets);

		if (timeReset == false) {
			timeReset = true;
			start = chrono::system_clock::now();
		}
		time = chrono::system_clock::now() - start;
		// 위치 보정
		if (time.count() >= 0.3) {
			// 위치 정보 계산결과 가져오고 push
			// 위치 push전에 Packet 조정 [0__0__10]
			PushPacket(alivePlayer, eventQueues, playerPackets);
			timeReset == false;
		}
	}
}