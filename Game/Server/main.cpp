#include "stdafx.h"

LockQueue<int32> q;

void Push()
{
	while (true) {
		int32 value = rand() % 100;
		q.Push(value);

		this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (true) {

		int32 data = 0;
		if (q.TryPop(OUT data)) {
			cout << data << endl;
		}


	}
}
int main()
{
	thread t1(Push);
	thread t2(Pop);
	thread t3(Pop);

	t1.join();
	t2.join();
	t3.join();
}