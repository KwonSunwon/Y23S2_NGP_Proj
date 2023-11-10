#pragma once
#include <memory>
#include "PacketManager.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...


PacketManager::PacketManager()
{
}

PacketManager::~PacketManager()
{
	// 소켓 닫기
	closesocket(m_sock);
	cout << "close Sock" << endl;
	// 윈속 종료
	WSACleanup();
}

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
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
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
}

void PacketManager::Reset()
{
	Packet packet;
	packet.stateMask = 0;
	send(m_sock, (char*)&packet, sizeof(packet), 0);
	cout << "소켓 종료" << endl;
	// 소켓 닫기
	closesocket(m_sock);
	cout << "close Sock" << endl;
	// 윈속 종료
	WSACleanup();
}

void PacketManager::SendPacket()
{
	//키 입력시 이 함수를 호출해서 서버로 send할 예정
	Packet packet;
	packet.stateMask = 0;
	packet.x = 1;
	packet.y = 1;
	int retval = send(m_sock, (char*)&packet, sizeof(packet), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

}

bool PacketManager::RecvPacket()
{
	//toClientEventque가 비어있지 않다면 모두 꺼내서 처리할 예정
	static bool gameStart = false;

	Packet packet;
	if (!gameStart) {
		int retval = recv(m_sock, (char*)&packet, sizeof(packet), 0);
		cout << "recv Packet" << retval << endl;
		gameStart = true;
	}


	return false;
}

shared_ptr<queue<Packet>> PacketManager::GetPacketQueue()
{
	return m_toClientEventQue;
}

