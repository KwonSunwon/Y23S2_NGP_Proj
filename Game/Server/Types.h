#pragma once

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

enum class GAME_LEVEL : BYTE {
	EASY,
	NORMAL,
	HARD,
	NONE
};

enum class STATE_MASK {
	RESULT,
	PLAYING,
	LIFE,
	POS_FLAG,
	PLAYER_NUM,
	GAME_START
};

struct Packet {
	float x, y;
	BYTE stateMask;
};

struct ClientInfo {
	shared_ptr<LockQueue<Packet>> toServerEventQueue;
	shared_ptr<LockQueue<Packet>> toClientEventQueue;
	GAME_LEVEL level;
	SOCKET sock;
};