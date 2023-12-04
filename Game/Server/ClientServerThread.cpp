#include "stdafx.h"
#include "ClientServerThread.h"
thread_local shared_ptr<LockQueue<Packet>> m_toServerEventQueue = make_shared<LockQueue<Packet>>();
thread_local shared_ptr<LockQueue<Packet>> m_toClientEventQueue = make_shared<LockQueue<Packet>>();;

void ClientServerThread(SOCKET client)
{
	Initialize(client);

	MainLoop(client);
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

	//최초 1회 난이도 수신
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

	//최초 1회 TLS로 이벤트큐를 2개 설정후
	//ClientInfoQueue에 넣어준다.
	ClientInfo clientInfo;
	clientInfo.level = level;
	clientInfo.sock = client;
	clientInfo.toClientEventQueue = m_toClientEventQueue;
	clientInfo.toServerEventQueue = m_toServerEventQueue;

	ClientInfoQueue[(int)level].Push(clientInfo);

	DWORD optval = 10;
	retval = setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&optval, sizeof(optval));

	optval = true;
	setsockopt(client, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval));
}

void MainLoop(SOCKET client)
{
	int retval;
	while (true) {
		//서버에서 클라이언트에 보낼 패킷이 있는 동안
		//전부 send
		{
			Packet packet;
			while (m_toClientEventQueue->TryPop(packet)) {
				retval = send(client, (char*)&packet, sizeof(packet), 0);
				//cout << retval;
				if (retval == SOCKET_ERROR) {
					//err_display("send()");
				}
			}
		}

		//recv() 로직 짜기
		{
			Packet packet;
			while (true) {
			#ifdef _DEBUG_CLIENT_SERVER
				cout << "recv Loop" << endl;
			#endif
				retval = recv(client, (char*)&packet, sizeof(packet), 0);
				if (retval > 0) {
					cout << packet.x << " " << packet.y << " " << packet.stateMask << endl;
					m_toServerEventQueue->Push(packet);
				}


				if (retval < 0) {
				#ifdef _DEBUG_CLIENT_SERVER
					cout << "TIMEOUT" << endl;
				#endif
					break;
	}
				if (retval == 0) {
					cout << "종료" << endl;
					packet.stateMask = 0;
					packet.x = numeric_limits<float>::infinity();
					packet.y = numeric_limits<float>::infinity();
					m_toServerEventQueue->Push(packet);
					return;
				}
}
	}
		//버퍼에 받은 패킷이 있으면 모두 서버큐로 푸쉬


		//보내기, 받기가 전부 완료되면 쓰레드 양보
		this_thread::yield();
}
	closesocket(client);
}
