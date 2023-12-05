#include "stdafx.h"
#include "InGameThread.h"

// 랜덤 시드를 위한 랜덤 엔진
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 15);

float elapsedTime = 0.f;
float totalTime = 0.f;

Physics ps;
CollisionManager cm;

// 쓰레드 함수내 쓰레드 초기화 함수
static void InitializeInGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER>* eventQueues, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerInfo, NUM_OF_PLAYER>* players)
{
	int seed = dis(gen);

	for (int i = 0; i < NUM_OF_PLAYER; ++i) {
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
		// 플레이어 초기 위치 저장
		(*players)[i].Pos.x = initialX[i];
		(*players)[i].Pos.y = initialY[i];
		// 랜덤 시드 값
		(*playerPackets)[i].stateMask |= seed;
		(*eventQueues)[i].toClientEventQueue->Push((*playerPackets)[i]);
	}
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
		for (int j = 0; j < NUM_OF_PLAYER; ++j)
			if (i != j)
				(*eventQueues)[i].toClientEventQueue->Push((*playerPackets)[j]);
}

// 초기 정보 전송 후 패킷 초기화
static void InitPacket(array<Packet, NUM_OF_PLAYER>* playerPackets)
{
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
	{
		(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::GAME_START);
		(*playerPackets)[i].stateMask |= (1 << (int)STATE_MASK::POS_FLAG);
		(*playerPackets)[i].stateMask |= (3 << (int)STATE_MASK::LIFE);
		(*playerPackets)[i].stateMask |= (1 << (int)STATE_MASK::PLAYING);
		(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::RESULT);
	}
}


// 큐에 데이터 Push
static void PushPacket(vector<int> alivePlayer, array<EventQueues, NUM_OF_PLAYER>* eventQueues, array<Packet, NUM_OF_PLAYER> playerPackets)
{
	for (auto player : alivePlayer)
	{
		for (int i = 0; i < NUM_OF_PLAYER; ++i)
		{
			(*eventQueues)[player].toClientEventQueue->Push(playerPackets[i]);
		}
	}
}

// 큐에서 데이터 Pop
static bool ToServerQueueCheck(vector<int> alivePlayer, array<EventQueues, NUM_OF_PLAYER>* eventQueues, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerInfo, NUM_OF_PLAYER>* players)
{
	int dataNum = alivePlayer.size();
	Packet tmp;
	for (auto player : alivePlayer)
	{
		if ((*eventQueues)[player].toServerEventQueue->TryPop(tmp) == true)
		{
			(*playerPackets)[player].x = tmp.x;
			(*playerPackets)[player].y = tmp.y;
			//(*players)[player].Acc.x = tmp.x;
			//(*players)[player].Acc.y = tmp.y;
			(*players)[player].ConstAcc.x = tmp.x;
			(*players)[player].ConstAcc.y = tmp.y;
		}
		else
		{
			dataNum--;
		}
	}
	if (dataNum < 1)
		return false;
	return true;
}

static void ResetAcc(vector<int> alivePlayer, array<PlayerInfo, NUM_OF_PLAYER>* players)
{
	for (auto player : alivePlayer)
	{
		(*players)[player].Acc.x = (*players)[player].ConstAcc.x;
		(*players)[player].Acc.y = (*players)[player].ConstAcc.y;
		if ((*players)[player].Acc.x != 0 && (*players)[player].Acc.y != 0) {
			(*players)[player].Acc.x = (*players)[player].Acc.x / ROOT_TWO;
			(*players)[player].Acc.y = (*players)[player].Acc.y / ROOT_TWO;
		}
	}
}

// 게임 강제 종료 체크
static void CheckPlayerExitGame(vector<int>* alivePlayer, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerInfo, NUM_OF_PLAYER>* players, array<EventQueues, NUM_OF_PLAYER>* eventQueues)
{
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
	{
		if ((*playerPackets)[i].x == numeric_limits<float>::infinity() &&
			(*playerPackets)[i].y == numeric_limits<float>::infinity())
		{
			(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::PLAYING);
			(*playerPackets)[i].stateMask &= ~(3 << (int)STATE_MASK::LIFE);
			(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::POS_FLAG);

			(*players)[i].Pos.x = 100.f;
			(*players)[i].Pos.y = 100.f;
			(*playerPackets)[i].x = (*players)[i].Pos.x;
			(*playerPackets)[i].y = (*players)[i].Pos.y;
			(*players)[i].Acc.x = 0;
			(*players)[i].Acc.y = 0;
			(*players)[i].Vel.x = 0;
			(*players)[i].Vel.y = 0;
			alivePlayer->erase(std::remove(alivePlayer->begin(), alivePlayer->end(), i), alivePlayer->end());
			cout << "Player" << i + 1 << " exit!" << alivePlayer->size() << endl;
			for (auto p : *alivePlayer)
				cout << p << endl;
			PushPacket(*alivePlayer, eventQueues, *playerPackets);
			(*playerPackets)[i].stateMask |= (3 << (int)STATE_MASK::LIFE);
		}
	}
}

// 게임 강제 종료 체크
static void CheckPlayerExitGame(vector<int>* alivePlayer, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerInfo, NUM_OF_PLAYER>* players)
{
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
	{
		if ((*playerPackets)[i].x == numeric_limits<float>::infinity() &&
			(*playerPackets)[i].y == numeric_limits<float>::infinity())
		{
			(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::PLAYING);
			(*playerPackets)[i].stateMask &= ~(3 << (int)STATE_MASK::LIFE);
			(*playerPackets)[i].stateMask &= ~(1 << (int)STATE_MASK::POS_FLAG);

			(*players)[i].Pos.x = 100.f;
			(*players)[i].Pos.y = 100.f;
			(*playerPackets)[i].x = (*players)[i].Pos.x;
			(*playerPackets)[i].y = (*players)[i].Pos.y;
			(*players)[i].Acc.x = 0;
			(*players)[i].Acc.y = 0;
			(*players)[i].Vel.x = 0;
			(*players)[i].Vel.y = 0;
			alivePlayer->erase(std::remove(alivePlayer->begin(), alivePlayer->end(), i), alivePlayer->end());
			cout << "Player" << i + 1 << " exit!" << alivePlayer->size() << endl;
			for (auto p : *alivePlayer)
				cout << p << endl;
		}
	}
}

// 승리시 승리 stateMask 비트 마스킹 후 큐에 Push
static void PushWinPacket(vector<int> alivePlayer, array<EventQueues, NUM_OF_PLAYER>* eventQueues, array<Packet, NUM_OF_PLAYER> playerPackets)
{
	playerPackets[*alivePlayer.begin()].stateMask |= (1 << (int)STATE_MASK::RESULT);
	playerPackets[*alivePlayer.begin()].stateMask |= (1 << (int)STATE_MASK::POS_FLAG);
	playerPackets[*alivePlayer.begin()].x = 0;
	playerPackets[*alivePlayer.begin()].y = 0;

	for (int i = 0; i < NUM_OF_PLAYER; ++i)
		(*eventQueues)[*alivePlayer.begin()].toClientEventQueue->Push(playerPackets[i]);
	cout << "[Player_" << *alivePlayer.begin() + 1 << "] Win!!!!!!!" << endl;
}

// 디버그 테스트용 print함수
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

static void ModifyPacketVel(vector<int> alivePlayer, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerInfo, NUM_OF_PLAYER>* players)
{
	for (auto player : alivePlayer)
	{
		(*playerPackets)[player].stateMask |= (1 << (int)STATE_MASK::POS_FLAG);
		(*playerPackets)[player].x = (*players)[player].Vel.x;
		(*playerPackets)[player].y = (*players)[player].Vel.y;
	}
}

static void ModifyPacketPos(vector<int> alivePlayer, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerInfo, NUM_OF_PLAYER>* players)
{
	for (auto player : alivePlayer)
	{
		(*playerPackets)[player].stateMask &= ~(1 << (int)STATE_MASK::POS_FLAG);
		(*playerPackets)[player].x = (*players)[player].Pos.x;
		(*playerPackets)[player].y = (*players)[player].Pos.y;
	}
}

//static void ModifyPacketLife(vector<int> alivePlayer, array<Packet, NUM_OF_PLAYER>* playerPackets, array<PlayerInfo, NUM_OF_PLAYER>* players)
//{
//	for (auto player : alivePlayer)
//	{
//		(*playerPackets)[0].stateMask &= ~(1 << (int)STATE_MASK::LIFE);
//		(*playerPackets)[player].x = (*players)[player].Pos.x;
//		(*playerPackets)[player].y = (*players)[player].Pos.y;
//	}
//}


void InGameThread(GAME_LEVEL level, array<EventQueues, NUM_OF_PLAYER> eventQueues)
{
#ifdef _DEBUG_INGAME
	cout << "InGame Thread Start" << endl;
#endif // _DEBUG_INGAME

	//bool timeReset = false;
	//chrono::system_clock::time_point start;
	//chrono::duration<double> time;

	array<PlayerInfo, NUM_OF_PLAYER> players;
	array<Packet, NUM_OF_PLAYER> playerPackets;
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
	std::chrono::system_clock::time_point prevTime = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point posPrevTime = std::chrono::system_clock::now();
#ifdef _DEBUG_INGAME
	cout << "초기 패킷데이터 확인" << endl;
	PrintPacketData(playerPackets);
#endif // _DEBUG_INGAME
	while (true) {
		//cout << totalTime << endl;
		auto now = std::chrono::system_clock::now();
		elapsedTime = static_cast<std::chrono::duration<double>>(now - prevTime).count();
		totalTime = static_cast<std::chrono::duration<double>>(now - posPrevTime).count();
		ToServerQueueCheck(alivePlayer, &eventQueues, &playerPackets, &players);
		CheckPlayerExitGame(&alivePlayer, &playerPackets, &players);
		if (alivePlayer.size() == 0)
			break;
		if (alivePlayer.size() == 1) {
			PushWinPacket(alivePlayer, &eventQueues, playerPackets);
			alivePlayer.clear();
			break;
		}
		/*if (totalTime > 5.0f) {
			cout << "목숨 패킷데이터 확인" << endl;
			ModifyPacketLife(alivePlayer, &playerPackets, &players);
			PrintPacketData(playerPackets);
			PushPacket(alivePlayer, &eventQueues, playerPackets);

			totalTime = -1000;
		}*/

		if (elapsedTime >= 0.0167f) {
			// 속도 계산
			ResetAcc(alivePlayer, &players);
			// 충돌 체크
			for (int i = 0; i < NUM_OF_PLAYER; ++i) {
				if (i == 0 || i == 1)
				{
					cm.DoCollideAB(&players[i], &players[i + 1]);
					if (i == 0) {
						cm.DoCollideAB(&players[i], &players[i + 2]);
					}
				}
				cm.DoCollideWithWall(&players[i]);
			}
			for (auto p : alivePlayer) {
				ps.CaculateVelocity(&players[p], elapsedTime);
			}
			// 위치 계산
			for (auto p : alivePlayer) {
				ps.CaculatePosition(&players[p]);
			}
			ModifyPacketVel(alivePlayer, &playerPackets, &players);
		#ifdef _DEBUG_INGAME
			cout << "속도 패킷데이터 확인" << endl;
			PrintPacketData(playerPackets);
		#endif // _DEBUG_INGAME
			PushPacket(alivePlayer, &eventQueues, playerPackets);
			//	// 위치 정보 계산결과 가져오고 push
			//	// 위치 push전에 Packet 조정 [0__0__10]
			prevTime = now;
		}
		if (totalTime >= 0.167f) {

			ModifyPacketPos(alivePlayer, &playerPackets, &players);
		#ifdef _DEBUG_INGAME
			cout << "위치 패킷데이터 확인" << endl;
			PrintPacketData(playerPackets);
		#endif // _DEBUG_INGAME
			PushPacket(alivePlayer, &eventQueues, playerPackets);
			posPrevTime = now;
		}
	}
}