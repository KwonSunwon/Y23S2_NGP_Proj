#include "stdafx.h"
#include "LobbyThread.h"

#include "InGameThread.h"

void LobbyThread()
{
#ifdef _DEBUG_LOOBY
	cout << "Lobby Thread Start" << endl;
#endif // _DEBUG_LOOBY
	while (true) {
		for (int level = 0; level < NUM_OF_LEVEL; level++) {
			if (ClientInfoQueue[level].Size() == NUM_OF_PLAYER) {
				// Game Start
				array<EventQueues, NUM_OF_PLAYER> eventQueues;
				ClientInfo tempClientInfo;
				for (auto& player : eventQueues) {
					ClientInfoQueue[level].WaitPop(tempClientInfo);
					player.toClientEventQueue = tempClientInfo.toClientEventQueue;
					player.toServerEventQueue = tempClientInfo.toServerEventQueue;
				}

				thread th = thread(/*InGameThread*/InGameThread, static_cast<GAME_LEVEL>(level), eventQueues);
				th.detach();
#ifdef _DEBUG_LOOBY
				cout << "Game Start" << endl;
#endif
			}
		}
	}
}