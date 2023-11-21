#pragma once
#include "vec2.h"

struct PlayerInfo {
	vec2f Vel;
	vec2f Acc;
	vec2f Pos;
	float Radius = 0;
	float Mass = 0;
};

class CollisionManager
{
private:
	PlayerInfo m_A;
	PlayerInfo m_B;

public:
	CollisionManager();
	~CollisionManager();

	bool DoCollideWithWall(PlayerInfo* A);
	bool DoCollideAB(PlayerInfo* A, PlayerInfo* B);

};