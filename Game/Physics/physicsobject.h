#pragma once
#include "buffer.h"
#include "loadobj.h"
#include "CollisionManager.h"



class Object
{
private:
	bool m_Up = false;
	bool m_Down = false;
	bool m_Left = false;
	bool m_Right = false;
	float m_Radius = 0.1f;
	float m_Mass;
	int num_triangle;
	glm::vec3 m_Color;
	glm::vec2 m_Acceleration = glm::vec2(0.f, 0.f);;
	glm::vec2 m_Position;
	glm::vec2 m_Velocity = glm::vec2(0.f, 0.f);
	BUFFER m_BallBuffer;
public:
	Object() {}

	Object(glm::vec2 pos, float mass, float radius, glm::vec3 color)
		: m_Position(pos), m_Mass(mass), m_Radius(radius), m_Color(color)
	{
		num_triangle = 0;
		memset(&m_BallBuffer, 0, sizeof(BUFFER));
	}

	~Object() {}

	Object operator=(Object other)
	{
		m_Radius = other.m_Radius;
		m_Mass = other.m_Mass;
		num_triangle = other.num_triangle;
		m_Color = other.m_Color;
		m_Acceleration = other.m_Acceleration;
		m_Velocity = other.m_Velocity;
		m_Position = other.m_Position;
		m_BallBuffer = other.m_BallBuffer;
		return *this;
	}

	void InitBuffer(GLuint shader_program)
	{
		num_triangle = loadObj_normalize_center("sphere.obj");

		glGenVertexArrays(1, &m_BallBuffer.vao);
		glBindVertexArray(m_BallBuffer.vao);
		glGenBuffers(2, m_BallBuffer.vbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_BallBuffer.vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, outvertex.size() * sizeof(glm::vec3), &outvertex[0], GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(shader_program, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, m_BallBuffer.vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, outnormal.size() * sizeof(glm::vec3), &outnormal[0], GL_STATIC_DRAW);
		GLint nAttribute = glGetAttribLocation(shader_program, "vNormal");
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(nAttribute);
	};
	void Draw(GLuint shader_program)
	{
		glm::vec3 position;
		position.x = m_Position.x;
		position.y = m_Position.y;
		position.z = 0;
		glm::mat4 transform(1.f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(m_Radius, m_Radius, m_Radius));
		GLint location = glGetUniformLocation(shader_program, "modelTransform");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

		int objColorLocation = glGetUniformLocation(shader_program, "objectColor");
		glUniform3f(objColorLocation, m_Color.x, m_Color.y, m_Color.z);


		glBindVertexArray(m_BallBuffer.vao);
		glDrawArrays(GL_TRIANGLES, 0, num_triangle);
	};

	void SetVelocity(glm::vec2 velocity) { m_Velocity = velocity; };
	void SetAccelerationX(float acceleration) { m_Acceleration.x = acceleration; };
	void SetAccelerationY(float acceleration) { m_Acceleration.y = acceleration; };
	void SetPosX(float x) { m_Position.x = x; };
	void SetPosY(float y) { m_Position.y = y; };
	glm::vec2 GetPos() { return m_Position; };
	glm::vec2 GetVelocity() { return m_Velocity; };
	glm::vec2 GetAcceleration() { return m_Acceleration; };
	float GetRadius() { return m_Radius; };
	float GetMass() { return m_Mass; };
	bool GetUp() { return m_Up; };
	bool GetDown() { return m_Down; };
	bool GetLeft() { return m_Left; };
	bool GetRight() { return m_Right; };
	void SetUp(bool val) { m_Up = val; };
	void SetDown(bool val) { m_Down = val; };
	void SetLeft(bool val) { m_Left = val; };
	void SetRight(bool val) { m_Right = val; };


	void SetAccelerationByKey()
	{
		m_Acceleration = glm::vec2(0, 0);
		if (m_Up) {
			if (m_Left || m_Right)
				m_Acceleration.y = (ACCELERATION / ROOT_TWO);
			else
				m_Acceleration.y = ACCELERATION;
		}
		if (m_Down) {
			if (m_Left || m_Right)
				m_Acceleration.y = -(ACCELERATION / ROOT_TWO);
			else
				m_Acceleration.y = -ACCELERATION;
		}
		if (m_Left) {
			if (m_Up || m_Down)
				m_Acceleration.x = -(ACCELERATION / ROOT_TWO);
			else
				m_Acceleration.x = -ACCELERATION;
		}
		if (m_Right) {
			if (m_Up || m_Down)
				m_Acceleration.x = (ACCELERATION / ROOT_TWO);
			else
				m_Acceleration.x = ACCELERATION;
		}
	};

	void VelocityUpdate()
	{
		m_Velocity = m_Velocity + m_Acceleration;

		glm::vec2 friction = -m_Velocity;
		float mag = sqrt(friction.x * friction.x + friction.y * friction.y);
		if (mag > FLT_EPSILON) {
			friction = friction / mag;
			friction = friction * COEF;

			glm::vec2 resultVel = m_Velocity + friction;
			if (resultVel.x * m_Velocity.x < 0.f)
				m_Velocity.x = 0.f;
			else
				m_Velocity.x = resultVel.x;
			if (resultVel.y * m_Velocity.y < 0.f)
				m_Velocity.y = 0.f;
			else
				m_Velocity.y = resultVel.y;
		}
		if (m_Velocity.x > MAX_SPEED)
			m_Velocity.x = MAX_SPEED;
		if (m_Velocity.x < -MAX_SPEED)
			m_Velocity.x = -MAX_SPEED;

		if (m_Velocity.y > MAX_SPEED)
			m_Velocity.y = MAX_SPEED;
		if (m_Velocity.y < -MAX_SPEED)
			m_Velocity.y = -MAX_SPEED;
		//cout << sqrt(m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y) << endl;
		if (m_Velocity.x == m_Velocity.y || m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y > MAX_SPEED * MAX_SPEED)
		{
			if (m_Velocity.x > MAX_SPEED / ROOT_TWO)
				m_Velocity.x = MAX_SPEED / ROOT_TWO;
			if (m_Velocity.x < -MAX_SPEED / ROOT_TWO)
				m_Velocity.x = -MAX_SPEED / ROOT_TWO;

			if (m_Velocity.y > MAX_SPEED / ROOT_TWO)
				m_Velocity.y = MAX_SPEED / ROOT_TWO;
			if (m_Velocity.y < -MAX_SPEED / ROOT_TWO)
				m_Velocity.y = -MAX_SPEED / ROOT_TWO;
		}
		//cout << sqrt(m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y) << endl;
	};

	void Update()
	{
		m_Position = m_Position + m_Velocity;
	};
};