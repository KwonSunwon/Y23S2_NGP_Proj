#include "stdafx.h"
#include "ClientServerThread.h"

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

	//closesocket(client);
}
