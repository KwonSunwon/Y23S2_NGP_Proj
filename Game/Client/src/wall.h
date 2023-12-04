#ifndef __WALL_H__
#define __WALL_H__

#include "object.h"
#include "stdafx.h"

class Wall : public Object
{
private:
	static GLint object;
	static vector<glm::vec3> vertices;
	static vector<glm::vec3> normals;
	static vector<glm::vec2> uvs;

	static unsigned int texture;

	static GLuint _vao;
	static GLuint _vbo;
	static GLuint _nbo;
	static GLuint _tbo;

	bool m_isAlive = false;

protected:
public:
	Wall(float, float);
	Wall(float);
	~Wall();
	void render(GLuint shaderProgramID) override;
	void colorInit();
	void initBuffer() override;
	void initTexture() override;

	void update() override;
	void move();
	void collision();

	void setAlive(bool isAlive) { m_isAlive = isAlive; };
};

#endif