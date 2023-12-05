#include "stdafx.h"
#include "LobbyThread.h"

#include "InGameThread.h"

void LobbyThread()
{
#ifdef _DEBUG_LOOBY
	cout << "Lobby Thread Start" << endl;
#endif // _DEBUG_LOOBY
	while (true) {
		if (ClientInfoQueue[0].Size() == NUM_OF_PLAYER) {
			// Game Start
			bool isAllReady = true;
			array<EventQueues, NUM_OF_PLAYER> eventQueues;
			ClientInfo tempClientInfo;
			vector<ClientInfo> infoQueue;
			for (auto& player : eventQueues) {
				ClientInfoQueue[0].WaitPop(tempClientInfo);
				// 연결이 끊긴 클라이언트가 있을 경우
				int error = 0;
				int len = sizeof(int);
				if (getsockopt(tempClientInfo.sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len) == 0) {
					if (error != 0) {
						cout << "Client Disconnected" << endl;
						isAllReady = false;
						continue;
					}
					infoQueue.emplace_back(tempClientInfo);
					player.toClientEventQueue = tempClientInfo.toClientEventQueue;
					player.toServerEventQueue = tempClientInfo.toServerEventQueue;
				}
			}

			if (!isAllReady) {
				cout << "Not All Ready" << endl;
				for (auto& player : infoQueue)
					ClientInfoQueue[0].Push(player);
			}
			else {
				cout << "All Ready" << endl;
				thread th = thread(/*InGameThread*/InGameThread, static_cast<GAME_LEVEL>(0), eventQueues);
				th.detach();
			}
		#ifdef _DEBUG_LOOBY
			cout << "Game Start" << endl;
		#endif // _DEBUG_LOOBY
		}
	}
}
