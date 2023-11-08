#include "stdafx.h"
#include "LobbyThread.h"

#include "InGameThread.h"

void LobbyThread()
{
	while (true) {
		for (int level = 0; level < NUM_OF_LEVEL; level++) {
			if (ClientInfoQueue[level].Size() == NUM_OF_PLAYER) {
				// Game Start
				//thread th = thread(/*InGameThread*/, static_cast<GAME_LEVEL>(level));
				//th.detach();
			}
		}
	}
}