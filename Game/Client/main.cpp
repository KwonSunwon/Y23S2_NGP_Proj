#include "src/stdafx.h"
#include "src/shaders.h"

#include "src/camera.h"
#include "src/object.h"
#include "src/light.h"
#include "src/player.h"
#include "src/gameWorld.h"
#include "src/wall.h"
#include "src/gameManager.h"

#include "src/PacketManager.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid keyUp(unsigned char, int, int);
GLvoid Mouse(int, int, int, int);

GLclampf g_color[4] = { 0.5, 0.5, 0.5, 1.0f };
GLint g_width = 1000, g_height = 1000;

GLuint shaderID;

extern Light light;
extern Camera camera;
extern GameManager gameManager;
extern GameWorld gameWorld;
extern int gameSpeed;

///////////////////////////////////////////

GLvoid updateTimer(int value);

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(g_width, g_height);
	glutCreateWindow("Last Project");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shaderID = initShader("res/shader.vert", "res/shader.frag");

	gameManager.gameRun();
	gameWorld.set_shader(shaderID);

	updateTimer(0);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(shaderID);

	gameManager.draw();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, 1000, 1000);
}

GLvoid keyboard(unsigned char key, int x, int y)
{
	gameManager.handleEvent(key, true);
	//SendPacket();

	glutPostRedisplay();

}
GLvoid keyUp(unsigned char key, int x, int y)
{
	gameManager.handleEvent(key, false);
	//g_PacketManager->SendPacket()

	glutPostRedisplay();
}
GLvoid Mouse(int button, int state, int x, int y)
{
	gameManager.handleEvent(button, state, x, y);
	glutPostRedisplay();
}
GLvoid updateTimer(int value)
{
	auto now = std::chrono::system_clock::now();
	g_elapsedTime = static_cast<std::chrono::duration<double>>(now - g_prevTime).count();
	g_prevTime = now;

	gameManager.update();
	glutTimerFunc(1000 / gameSpeed, updateTimer, 0);
}