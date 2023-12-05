#ifndef __BG_H__
#define __BG_H__

#include "object.h"

enum class BG_TYPE {
	MAIN = 1,
	WIN,
	LOOSE,
};

class BG : public Object
{
private:
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> uvs;

	static GLuint texture[3];

public:
	BG();
	void render(GLuint shaderProgramID) override;
	void initBuffer() override;
	void initTexture() override;

	BG_TYPE type = BG_TYPE::MAIN;
};

#endif