#include "stdafx.h"
#include "InGameThread.h"
#include "ClientServerThread.h"
#include "LobbyThread.h"

void print2Digit(BYTE num)
{
	for (int i = 7; i >= 0; --i) { //8자리 숫자까지 나타냄
		int result = num >> i & 1;
		printf("%d", result);
	}
	printf("\n");
}

int main()
{
	thread lobbyThreadHandle;
	lobbyThreadHandle = thread(LobbyThread);

	//---------------------------------------
	// wait sock 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(0);
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		exit(0);
	struct sockaddr_in serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(9000);
	if (bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		exit(0);
	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
		exit(0);
	//---------------------------------------
	// client accept loop
	SOCKET clientSock;
	struct sockaddr_in clientAddr;
	int addrLen;
	thread clientServerThreadHandle;

	printf("Server start, waiting client...\n");
	while (true) {
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET) {
			continue;
		}

		clientServerThreadHandle = thread(/*Client-Server Thread*/ClientServerThread, clientSock);
		// detach()를 호출해서 thread를 떼어내야 하나의 handle로 여러 thread를 생성가능
		clientServerThreadHandle.detach();
	}

	closesocket(listenSock);
	WSACleanup();
}