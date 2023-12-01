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
CollisionManager Cm;
PlayerInfo Players[NUM_OF_PLAYER];


void UpdatePlayerInfo(PlayerInfo* Player, Object* obs)
{
	Player->Acc.x = obs->GetAcceleration().x;
	Player->Acc.y = obs->GetAcceleration().y;
	Player->Pos.x = obs->GetPos().x;
	Player->Pos.y = obs->GetPos().y;
	Player->Vel.x = obs->GetVelocity().x;
	Player->Vel.y = obs->GetVelocity().y;
	Player->Radius = obs->GetRadius();
	Player->Mass = obs->GetMass();
}

void CheckColideWithOtherPlayer(PlayerInfo* Player1, PlayerInfo* Player2, Object* obs1, Object* obs2)
{
	Cm.DoCollideAB(Player1, Player2);
	if (Cm.DoCollideAB(Player1, Player2))
	{
		obs1->SetAccelerationX(Player1->Acc.x);
		obs1->SetAccelerationY(Player1->Acc.y);

		obs2->SetAccelerationX(Player2->Acc.x);
		obs2->SetAccelerationY(Player2->Acc.y);
	}

	//if (Cm.DoCollideWithWall(Player2)) {
	//	obs->SetAccelerationX(Player2->Acc.x);
	//	obs->SetAccelerationY(Player2->Acc.y);
	//}
}

void CheckColideWithWall(PlayerInfo* Player, Object* obs)
{
	if (Cm.DoCollideWithWall(Player)) {
		obs->SetAccelerationX(Player->Acc.x);
		obs->SetAccelerationY(Player->Acc.y);
		//obs->SetVelocity(glm::vec2(Player->Vel.x, Player->Vel.y));
	}
}

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
		Objects[0].SetUp(true);
		break;
	case 'a':
		Objects[0].SetLeft(true);
		break;
	case 's':
		Objects[0].SetDown(true);
		break;
	case 'd':
		Objects[0].SetRight(true);
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
		Objects[0].SetUp(false);
		break;
	case 'a':
		Objects[0].SetLeft(false);
		break;
	case 's':
		Objects[0].SetDown(false);
		break;
	case 'd':
		Objects[0].SetRight(false);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	//TimerFunc();
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
	{
		Objects[i].SetAccelerationByKey();
		UpdatePlayerInfo(&Players[i], &Objects[i]);
	}
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
	{
		if (i == 0 || i == 1)
		{
			CheckColideWithOtherPlayer(&Players[i], &Players[(i + 1)], &Objects[i], &Objects[i + 1]);
			if (i == 0)
				CheckColideWithOtherPlayer(&Players[i], &Players[(i + 2)], &Objects[i], &Objects[i + 2]);
		}
		CheckColideWithWall(&Players[i], &Objects[i]);
		Objects[i].VelocityUpdate();
	}
	for (int i = 0; i < NUM_OF_PLAYER; ++i)
		Objects[i].Update();

	//cout << "x=" << Objects[0].GetPos().x << "y=" << Objects[0].GetPos().y << endl;
	//cout << "velocity=" << sqrt(Objects[0].GetVelocity().x * Objects[0].GetVelocity().x + Objects[0].GetVelocity().y * Objects[0].GetVelocity().y) << endl;
	glutPostRedisplay();// 화면 재 출력
	glutTimerFunc(1000 / gameSpeed, TimerFunction, value);// 타이머함수 재 설정

}