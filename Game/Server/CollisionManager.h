#pragma once
#include "vec2.h"
#include "physics.h"

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