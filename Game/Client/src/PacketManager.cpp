#pragma once
#include <memory>
#include "PacketManager.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

//-----------------------------------------------------------------
//에러 처리 함수
//-----------------------------------------------------------------
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
//----------------------------------------------------------------------



//----------------------------------------------------------------------
//패킷 매니저
//----------------------------------------------------------------------
PacketManager::PacketManager()
{
}

PacketManager::~PacketManager()
{
	// 소켓 정리
	closesocket(m_sock);
	cout << "close Sock" << endl;
	WSACleanup();
}

void PacketManager::Initialize(GAME_LEVEL level)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		err_quit("Client Socket Init Error");
	}

	// 소켓 생성
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) {
		err_quit("socket()");
	}

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, m_serverIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connect()");
	}

	//최초 난이도 제공용 send
	retval = send(m_sock, (char*)&level, sizeof(level), 0);
	cout << retval;
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	bool optval = true;
	setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval));
}

#include<bitset>
void PacketManager::SendPacket(BYTE flag, float x, float y)
{
	//키 입력시 이 함수를 호출해서 서버로 send할 예정
	Packet packet;

	packet.stateMask = flag;
	packet.x = x;
	packet.y = y;
	cout << "\nsendPacket x:" << x << " y:" << y << " bit:";
	cout << bitset<8>(packet.stateMask);
	int retval = send(m_sock, (char*)&packet, sizeof(Packet), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

}

bool PacketManager::RecvPacket(Packet* packet)
{
	int retval = recv(m_sock, (char*)packet, sizeof(Packet), 0);
	if (retval < 0) {
		return false;
	}
	return true;
}

void PacketManager::SetSocketOpt()
{
	DWORD optval = 10;
	int retval = setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&optval, sizeof(optval));
}

void PacketManager::SetIPAddress(char* ip)
{
	m_serverIP = ip;
}


void PacketManager::WaitForPlayers()
{
	for (int i = 0; i < 3; i++) {
		Packet* packet = new Packet();
		g_PacketManager->RecvPacket(packet);
		m_initPacket[i] = *packet;
	}
	SetEvent(g_connectionEvent);
}

void PacketManager::SendClosePacket()
{
	Packet packet;
	packet.stateMask = 0;
	packet.x = 777;
	packet.y = 777;
	send(m_sock, (char*)&packet, sizeof(Packet), 0);
}

