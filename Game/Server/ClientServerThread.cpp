#include "stdafx.h"
#include "ClientServerThread.h"
thread_local shared_ptr<LockQueue<Packet>> m_toServerEventQueue = make_shared<LockQueue<Packet>>();
thread_local shared_ptr<LockQueue<Packet>> m_toClientEventQueue = make_shared<LockQueue<Packet>>();;

void ClientServerThread(SOCKET client)
{
	Initialize(client);

	
}

void Initialize(SOCKET client)
{
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[24];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));


	GAME_LEVEL level = GAME_LEVEL::NONE;
	int retval = recv(client, (char*)&level, sizeof(level), 0);
	switch (level) {
	case GAME_LEVEL::NONE:
		cout << "NONE" << endl;
		break;

	case GAME_LEVEL::EASY:
		cout << "EASY" << endl;
		break;

	case GAME_LEVEL::NORMAL:
		cout << "NORMAL" << endl;
		break;

	case GAME_LEVEL::HARD:
		cout << "HARD" << endl;
		break;

	}
	ClientInfo clientInfo;
	clientInfo.level = level;
	clientInfo.sock = client;
	clientInfo.toClientEventQueue = m_toClientEventQueue;
	clientInfo.toServerEventQueue = m_toServerEventQueue;
	
	ClientInfoQueue.Push(clientInfo);
	

	//closesocket(client);
}
