#include "stdafx.h"
#include "shader.h"
#include "physicsobject.h"



GLvoid DrawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

void InitBuffer();

GLint screen_w = 800, screen_h = 600;

float cameraRotateY = 0.1f;
float cameraPosX = 0.5f;
float cameraPosZ = 200.f;

int gameSpeed = 60;

bool UpCompressed = false;
bool DownCompressed = false;
bool LeftCompressed = false;
bool RightCompressed = false;


GLuint shader_program;
//GLchar* vertexsource, * fragmentsource; //--- 소스코드 저장 변수
//GLuint vertexshader, fragmentshader; //--- 세이더 객체

// 초기 위치
const float initialX[3] = { 0, -0.3, 0.3 };
const float initialY[3] = { 0.3, -0.3, -0.3 };
glm::vec3 initialColor[3] = {
	glm::vec3(0.f,1.f,0.f),
	glm::vec3(1.f,0.f,0.f),
	glm::vec3(1.f,0.f,0.f)
};

Object Objects[NUM_OF_PLAYER];

int main(int argc, char** argv) //윈도우 출력하고 콜백함수 설정
{
	for (int i = 0; i < 3; ++i) {
		Objects[i] = Object(glm::vec2(initialX[i], initialY[i]), 0.5f, 0.1f, initialColor[i]);
	}

	glutInit(&argc, argv);// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(screen_w, screen_h); // 윈도우의 크기 지정
	glutCreateWindow("Physics"); // 윈도우 생성 윈도우 이름

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)// glew 초기화
	{
		std::cout << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized \n";

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);

	InitShader(shader_program); //--- 세이더 프로그램 만들기
	InitBuffer();

	glutDisplayFunc(DrawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);// 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutTimerFunc(0, TimerFunction, 1);



	glutMainLoop();// 이벤트 처리 시작
}


void InitBuffer()
{
	for (int i = 0; i < 3; ++i)
		Objects[i].InitBuffer(shader_program);
}

void DrawScene() {

	//--- 변경된 배경색 설정
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_w, screen_h);

	// 카메라
	GLint viewTransform = glGetUniformLocation(shader_program, "viewTransform");
	glm::mat4 viewMat = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(viewTransform, 1, GL_FALSE, glm::value_ptr(viewMat));

	// 원근 투영
	GLint projTransform = glGetUniformLocation(shader_program, "projTransform");
	glm::mat4 projMat = glm::perspective(glm::radians(45.f), (float)screen_w / (float)screen_h, 0.1f, 10000.f);
	glUniformMatrix4fv(projTransform, 1, GL_FALSE, glm::value_ptr(projMat));

	// 빛
	int lightPosLocation = glGetUniformLocation(shader_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.f, 0.8f, 0.0f);
	int lightColorLocation = glGetUniformLocation(shader_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.f, 1.f, 1.f);

	for (int i = 0; i < 3; ++i)
		Objects[i].Draw(shader_program);

	glutSwapBuffers(); //--- 화면에 출력하기
}



void Reshape(int w, int h) {
	screen_w = w;
	screen_h = h;
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		if (DownCompressed == false)
		{
			UpCompressed = true;
			if (LeftCompressed)
				Objects[0].SetAcceleration(glm::vec2(-ACCELERATION, ACCELERATION));
			else if (RightCompressed)
				Objects[0].SetAcceleration(glm::vec2(ACCELERATION, ACCELERATION));
			else
				Objects[0].SetAcceleration(glm::vec2(0.f, ACCELERATION));
		}
		break;
	case 'a':
		if (RightCompressed == false)
		{
			LeftCompressed = true;
			if (UpCompressed)
				Objects[0].SetAcceleration(glm::vec2(-ACCELERATION, ACCELERATION));
			else if (DownCompressed)
				Objects[0].SetAcceleration(glm::vec2(-ACCELERATION, -ACCELERATION));
			else
				Objects[0].SetAcceleration(glm::vec2(-ACCELERATION, 0.f));
		}
		break;
	case 's':
		if (UpCompressed == false)
		{
			DownCompressed = true;
			if (LeftCompressed)
				Objects[0].SetAcceleration(glm::vec2(-ACCELERATION, -ACCELERATION));
			else if (RightCompressed)
				Objects[0].SetAcceleration(glm::vec2(ACCELERATION, -ACCELERATION));
			else
				Objects[0].SetAcceleration(glm::vec2(0.f, -ACCELERATION));
		}
		break;
	case 'd':
		if (LeftCompressed == false)
		{
			RightCompressed = true;
			if (UpCompressed)
				Objects[0].SetAcceleration(glm::vec2(ACCELERATION, ACCELERATION));
			else if (DownCompressed)
				Objects[0].SetAcceleration(glm::vec2(ACCELERATION, -ACCELERATION));
			else
				Objects[0].SetAcceleration(glm::vec2(ACCELERATION, 0.f));
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	glm::vec2 temp = Objects[0].GetVelocity();
	glm::vec2 tempAcc = Objects[0].GetAcceleration();
	switch (key)
	{
	case 'w':
		UpCompressed = false;
		Objects[0].SetVelocity(glm::vec2(temp.x, 0.f));
		Objects[0].SetAcceleration(glm::vec2(tempAcc.x, 0.f));
		break;
	case 'a':
		LeftCompressed = false;
		Objects[0].SetVelocity(glm::vec2(0.f, temp.y));
		Objects[0].SetAcceleration(glm::vec2(0.f, tempAcc.y));
		break;
	case 's':
		DownCompressed = false;
		Objects[0].SetVelocity(glm::vec2(temp.x, 0.f));
		Objects[0].SetAcceleration(glm::vec2(tempAcc.x, 0.f));
		break;
	case 'd':
		RightCompressed = false;
		Objects[0].SetVelocity(glm::vec2(0.f, temp.y));
		Objects[0].SetAcceleration(glm::vec2(0.f, tempAcc.y));
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	//TimerFunc();
	for (int i = 0; i < 3; ++i)
		Objects[i].Update();
	glutPostRedisplay();// 화면 재 출력
	glutTimerFunc(1000 / gameSpeed, TimerFunction, value);// 타이머함수 재 설정

}