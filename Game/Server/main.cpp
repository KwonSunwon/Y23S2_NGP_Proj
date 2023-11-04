#include "stdafx.h"
#include "InGameThread.h"

//LockQueue<int32> q;
//
//void Push()
//{
//	while (true) {
//		int32 value = rand() % 100;
//		q.Push(value);
//
//		this_thread::sleep_for(10ms);
//	}
//}
//
//void Pop()
//{
//	while (true) {
//
//		int32 data = 0;
//		if (q.TryPop(OUT data)) {
//			cout << data << endl;
//		}
//
//
//	}
//}

void print2Digit(BYTE num)
{
	for (int i = 7; i >= 0; --i) { //8자리 숫자까지 나타냄
		int result = num >> i & 1;
		printf("%d", result);
	}
	printf("\n");
}

int main()
{
	//thread t1(Push);
	//thread t2(Pop);
	//thread t3(Pop);

	//t1.join();
	//t2.join();
	//t3.join();

	// 인게임thread초기화 test
	initClientInfoQueue();
	BYTE level;
	shared_ptr<shared_ptr<LockQueue<Packet>>[]> eventQueuePtr(new shared_ptr<LockQueue<Packet>>[3]);
	eventQueuePtr = InitializeInGameThread(&level);
	printf("level = %d\n", level);
	Packet pack;
	for (int i = 0; i < 3; ++i){
		eventQueuePtr[i]->TryPop(pack);
		printf("%.1f, %.1f \n", pack.x, pack.y);
		print2Digit(pack.stateMask);
	}
}