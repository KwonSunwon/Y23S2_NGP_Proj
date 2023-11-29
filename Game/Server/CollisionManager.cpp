#include "stdafx.h"
#include "CollisionManager.h"

Physics Ps;

CollisionManager::CollisionManager() {};

CollisionManager::~CollisionManager() {};

bool CollisionManager::DoCollideWithWall(PlayerInfo* A)
{


	if (A->Pos.x > END_OF_X - A->Radius)
	{
		Ps.AfterColideWithWall(&A->Acc.x, &A->Vel.x, A->Pos.x + A->Radius - END_OF_X);
		return true;
	}
	else if (A->Pos.x < -END_OF_X + A->Radius)
	{
		Ps.AfterColideWithWall(&A->Acc.x, &A->Vel.x, A->Pos.x - A->Radius + END_OF_X);
		return true;
	}
	if (A->Pos.y > END_OF_Y - A->Radius)
	{
		Ps.AfterColideWithWall(&A->Acc.y, &A->Vel.y, A->Pos.y + A->Radius - END_OF_Y);
		return true;
	}
	else if (A->Pos.y < -END_OF_Y + A->Radius)
	{
		Ps.AfterColideWithWall(&A->Acc.y, &A->Vel.y, A->Pos.y - A->Radius + END_OF_Y);
		return true;
	}
	return false;
}

bool CollisionManager::DoCollideAB(PlayerInfo* A, PlayerInfo* B)
{
	float xDist = A->Pos.x - B->Pos.x;
	float yDist = A->Pos.y - B->Pos.y;
	float totalDist = xDist * xDist + yDist * yDist;
	float radiusSum = A->Radius + B->Radius;
	float radiusError = A->Radius + B->Radius - MAX_SPEED;

	if (totalDist <= radiusSum * radiusSum)
	{
		//cout << "충돌 일어남! " << totalDist << " " << radiusSum * radiusSum << endl;
		//Ps.AfterColideWithPlayer(&A->Acc, &B->Acc, A->Pos, B->Pos, A->Vel, B->Vel, A->Mass, B->Mass);		
		Ps.AfterColideWithPlayer(A, B);
		return true;
	}
	return false;
}