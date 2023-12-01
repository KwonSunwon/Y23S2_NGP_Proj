#pragma once
#include "vec2.h"

struct PlayerInfo {
	vec2f Vel;
	vec2f Acc;
	vec2f Pos;
	float Radius = PLAYER_RADIUS;
	float Mass = PLAYER_MASS;
};

class Physics
{
private:
	// ¸¶Âû°è¼ö
	//const float m_COF;
	//glm::vec2 m_Vel[NUM_OF_PLAYER];
	//glm::vec2 m_Acc[NUM_OF_PLAYER];
	//glm::vec2 m_Pos[NUM_OF_PLAYER];
public:
	Physics();
	~Physics();


	void CaculateVelocity(PlayerInfo* players);
	void CaculatePosition(PlayerInfo* players);

	void AfterColideWithWall(float* Acc, float* Vel, float diff);
	//void AfterColideWithPlayer(vec2f* AccA, vec2f* AccB, vec2f posA, vec2f posB, vec2f velA, vec2f velB, const float massA, const float massB);	
	void AfterColideWithPlayer(PlayerInfo* A, PlayerInfo* B);

	vec2f DecideUnitVec(vec2f posA, vec2f posB);
	vec2f AfterColisionVelocityA(vec2f velA, vec2f velB, vec2f unitVec, float massA, float massB);
	vec2f AfterColisionVelocityB(vec2f velA, vec2f velB, vec2f unitVec, float massA, float massB);
	void AfterForce(vec2f* AccA, vec2f* AccB, vec2f unitVec, float massA, float massB);
};