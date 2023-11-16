#include "physics.h"
#include "stdafx.h"

Physics::Physics() { };

Physics::~Physics() {};

void Physics::AfterColideWithWall(float* Acc, float* Vel)
{
	*Acc = (- *Vel * 2.1) - *Acc;
}

void Physics::AfterColideWithPlayer(vec2f* AccA, vec2f* AccB, vec2f posA, vec2f posB, vec2f velA, vec2f velB, const float massA, const float massB)
{
	vec2f unitVec = DecideUnitVec(posA, posB);
	vec2f velAAfter = AfterColisionVelocityA(velA, velB, unitVec, massA, massB);
	vec2f velBAfter = AfterColisionVelocityB(velA, velB, unitVec, massA, massB);
	*AccA = velAAfter - velA;
	*AccB = velBAfter - velB;
	cout << velAAfter.x << ", " << velAAfter.y << endl;
}

vec2f Physics::DecideUnitVec(vec2f posA, vec2f posB)
{		//위치차이 단위벡터
	float x = posA.x - posB.x;
	float y = posA.y - posB.y;

	float nor_x = x / sqrt(x * x + y * y);
	float nor_y = y / sqrt(x * x + y * y);
	return vec2f(nor_x, nor_y);
}

// 완전 탄성 일 때 운동량 보존의 법칙
vec2f Physics::AfterColisionVelocityA(vec2f velA, vec2f velB, vec2f unitVec, float massA, float massB)
{
	vec2f vel1 = unitVec * vec2f().dot(velA, unitVec);
	vec2f vel2 = unitVec * vec2f().dot(velB, unitVec);
	vec2f velSub = vel1 - vel2;
	vec2f J =  (velSub * -1 * massB) / ((massA + massB) / 2);
	vec2f vel1After = vel1 + J;
	return vel1After;
}

vec2f Physics::AfterColisionVelocityB(vec2f velA, vec2f velB, vec2f unitVec, float massA, float massB)
{
	vec2f vel1 = unitVec * vec2f().dot(velA, unitVec);
	vec2f vel2 = unitVec * vec2f().dot(velB, unitVec);
	vec2f velSub = vel1 - vel2;
	vec2f J = (velSub * massA) / ((massA + massB) / 2);
	vec2f vel2After = vel2 + J;
	return vel2After;
}