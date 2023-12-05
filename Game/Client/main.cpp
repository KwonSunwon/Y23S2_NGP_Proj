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

#include "resource.h"

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

void DlgThread();
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_inst;
HWND g_dlg;
HWND g_ipEdit, g_textEdit;
LPARAM g_dlgServerIP = 0x7f000001;

void main(int argc, char** argv)
{
	g_inst = GetModuleHandle(NULL);
	thread dlgThread(DlgThread);
	dlgThread.detach();
	WaitForSingleObject(g_connectionEvent, INFINITE);
	g_PacketManager->WaitForPlayers();
	WaitForSingleObject(g_connectionEvent, INFINITE);
	EndDialog(g_dlg, IDOK);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(g_width, g_height);
	glutCreateWindow("VOYAGE IN SPACE");
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
	auto now = std::chrono::system_clock::now();
	g_elapsedTime = static_cast<std::chrono::duration<double>>(now - g_prevTime).count();
	g_prevTime = now;

	gameManager.update();

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

	glutTimerFunc(0.0001, updateTimer, 0);
}

void DlgThread()
{
	g_dlg = CreateDialog(g_inst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	ShowWindow(g_dlg, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		WaitForSingleObject(g_connectionEvent, 0);
	}
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT desktopRect;
	RECT dialogRect;
	int dialogWidth;
	int dialogHeight;
	int centerX;
	int centerY;

	switch (uMsg) {
	case WM_INITDIALOG:
		SetWindowText(hDlg, L"VOYAGE IN SPACE");
		desktopRect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopRect, 0);
		dialogRect;
		GetWindowRect(hDlg, &dialogRect);
		dialogWidth = dialogRect.right - dialogRect.left;
		dialogHeight = dialogRect.bottom - dialogRect.top;
		centerX = (desktopRect.left + desktopRect.right - dialogWidth) / 2;
		centerY = (desktopRect.top + desktopRect.bottom - dialogHeight) / 2;
		SetWindowPos(hDlg, NULL, centerX, centerY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		g_ipEdit = GetDlgItem(hDlg, IDC_IPADDRESS1);
		g_textEdit = GetDlgItem(hDlg, IDC_EDIT1);
		SendMessage(g_ipEdit, IPM_SETADDRESS, 0, g_dlgServerIP);
		SendMessage(g_textEdit, WM_SETTEXT, 0, (LPARAM)L"IP를 입력하고 접속 버튼을 눌러주세요.");
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			SendMessage(g_ipEdit, IPM_GETADDRESS, 0, (LPARAM)&g_dlgServerIP);
			g_PacketManager->SetIPAddress(PacketManager::LPARAMToCharPtr(g_dlgServerIP));
			g_PacketManager->Initialize(GAME_LEVEL::EASY);
			SendMessageW(g_textEdit, WM_SETTEXT, 0, (LPARAM)L"다른 플레이어를 기다리는 중...");
			SetEvent(g_connectionEvent);
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			exit(0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		exit(0);
		break;
	}
	return FALSE;
}