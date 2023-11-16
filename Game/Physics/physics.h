#pragma once
#include "vec2.h"

class Physics
{
private:
	// 마찰계수
	//const float m_COF;
	//glm::vec2 m_Vel[NUM_OF_PLAYER];
	//glm::vec2 m_Acc[NUM_OF_PLAYER];
	//glm::vec2 m_Pos[NUM_OF_PLAYER];
public:
	Physics();
	~Physics();


	void AfterColideWithWall(float* Acc, float* Vel);
	void AfterColideWithPlayer(vec2f* AccA, vec2f* AccB, vec2f posA, vec2f posB, vec2f velA, vec2f velB, const float massA, const float massB);
	
	vec2f DecideUnitVec(vec2f posA, vec2f posB);
	vec2f AfterColisionVelocityA(vec2f velA, vec2f velB, vec2f unitVec, float massA, float massB);
	vec2f AfterColisionVelocityB(vec2f velA, vec2f velB, vec2f unitVec, float massA, float massB);
};

//float dot(glm::vec2 a, glm::vec2 b) {
//	return a.x * b.x + a.y * b.y;
//}
//
//float decide_friction_force(float m, float g, float c) {// c는 마찰계수, 마찰력계산
//	return c;
//}
//

//
//glm::vec2 decide_acceleration(glm::vec2 f, float m) {	//c는 마찰력 가속도 계산 - 마찰포함
//	return glm::vec2(f.x / m, f.y / m);
//}
//
//float vector_size(glm::vec2 v) {
//	return sqrt((v.x * v.x) + (v.y * v.y));
//}
//
//glm::vec2 decide_velocity(glm::vec2 v, glm::vec2 a, float m, float c) {		//속도계산 C는 마찰
//	/*glm::vec2 unit_vec = glm::vec2(0.f, 0.f);
//	if ((int)vector_size(v) != 0) {
//		glm::vec2 unit_vec(v.x / vector_size(v), v.y / vector_size(v));
//	}
//	glm::vec2 fric_vec ((c / m) *  (-unit_vec.x), (c / m) * (-unit_vec.y));*/
//	return glm::vec2(v.x / (1.0 + c), v.y / (1.0 + c));
//	/*glm::vec2 new_vel(v.x + fric_vec.x, v.y + fric_vec.y);
//	if (vector_size(new_vel) <= 0.1)
//		return glm::vec2(0.f, 0.f);
//	return new_vel;*/
//}
//
//glm::vec2 decide_unit_vec(glm::vec3 b1, glm::vec3 b2) {		//위치차이 단위벡터
//	float x = b2.x - b1.x;
//	float y = b2.z - b1.z;
//
//	float nor_x = x / sqrt(x * x + y * y);
//	float nor_y = y / sqrt(x * x + y * y);
//	return glm::vec2(nor_x, nor_y);
//}
//
//
//glm::vec2 after_colision_velocity1(glm::vec2 v_in, glm::vec2 v_out, glm::vec2 n, float m, float e) {	//n은 단위벡터
//	glm::vec2 v1 = dot(v_in, n) * n;
//	glm::vec2 v2 = dot(v_out, n) * n;
//	glm::vec2 vr = v1 - v2;
//	glm::vec2 J = -vr * (e + 1) * (m / 2);
//	glm::vec2 v1_after = v_in + J / m;
//	return v1_after;
//}
//
//glm::vec2 after_colision_velocity2(glm::vec2 v_in, glm::vec2 v_out, glm::vec2 n, float m, float e) {	//n은 단위벡터
//	glm::vec2 v1 = dot(v_in, n) * n;
//	glm::vec2 v2 = dot(v_out, n) * n;
//	glm::vec2 vr = v1 - v2;
//	glm::vec2 J = -vr * (e + 1) * (m / 2);
//	glm::vec2 v2_after = v_out - J / m;
//	return v2_after;
//}
//
//glm::vec2 after_colision_wall(glm::vec2 v_in, float y_degree) {
//	float v_size = sqrt(v_in.x * v_in.x + v_in.y * v_in.y);
//
//	glm::vec3 norm(1.f, 0.f, 0.f);
//	glm::mat4 transform(1.f);
//	transform = glm::rotate(transform, glm::radians(y_degree), glm::vec3(0, 1, 0));
//
//	norm = transform * glm::vec4(norm.x, norm.y, norm.z, 1.f);
//	glm::vec2 r_norm(norm.x, norm.z);
//	float r_norm_size = sqrt(r_norm.x * r_norm.x + r_norm.y * r_norm.y);
//
//	float pi = 3.141592;
//
//	float theta = glm::acos(dot(v_in, r_norm) / (v_size * r_norm_size)) * (180 / pi);
//
//
//	glm::mat4 vel_transform(1.f);
//	vel_transform = glm::rotate(transform, glm::radians(180.f - 2 * theta), glm::vec3(0, 1, 0));
//	glm::vec3 v(v_in.x, 0, v_in.y);
//	v = vel_transform * glm::vec4(v.x, v.y, v.z, 1.f);
//
//	return glm::vec2(v.x, v.z);
//}