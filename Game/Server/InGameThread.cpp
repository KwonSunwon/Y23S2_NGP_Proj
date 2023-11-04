#include "stdafx.h"
#include "InGameThread.h"

// 랜덤 시드를 위한 랜덤 엔진
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 15);

// 임시 ClientInfoQueue
LockQueue<ClientInfo> ClientInfoQueue;
void initClientInfoQueue()
{
	ClientInfo a[3];
	for (int i = 0; i < 3; ++i) {
		shared_ptr<LockQueue<Packet>> p = make_shared<LockQueue<Packet>>();
		a[i].level = (BYTE)GAME_LEVEL::EASY;
		a[i].packetQueptr = p;
		a[i].sock = 0;
		ClientInfoQueue.Push(a[i]);
	}
}
// ---------------------------------------

// 쓰레드 함수내 쓰레드 초기화 함수
shared_ptr<shared_ptr<LockQueue<Packet>>[]> InitializeInGameThread(BYTE* level)
{
	
	// 주소를 담을 쓰레드내 변수가 필요
	shared_ptr<shared_ptr<LockQueue<Packet>>[]> tmpPtr(new shared_ptr<LockQueue<Packet>>[3]);
	ClientInfo info;
	Packet pack;
	int seed = dis(gen);

	for (int i = 0; i < 3; ++i) {
		// 난이도
		// ClientInfoQueue에서 Packet 데이터 pop
		ClientInfoQueue.TryPop(info);
		// 소켓 이부분은 각 쓰레드 생성할 때 하고 따로 전송필요가 있는지
		//*sock = info.sock;
		// stateMask 초기화
		pack.stateMask = 0;
		// 시작 상태
		pack.stateMask |= (1 << 7);
		// 플레이어 번호
		pack.stateMask |= (i << 5);
		// 초기위치설정, opengl로 좌표변환은 Client에서
		pack.stateMask &= ~(1 << 4);
		pack.x = initialX[i];
		pack.y = initialY[i];
		// 랜덤 시드 값
		pack.stateMask |= seed;
		// toClientEventQueue
		info.packetQueptr->Push(pack);
		tmpPtr[i] = info.packetQueptr;
	}
	// 난이도
	*level = (BYTE)info.level;
	return tmpPtr;
}