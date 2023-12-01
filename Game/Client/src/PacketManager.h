#ifndef __PACKET_MANAGER_H__
#define __PACKET_MANAGER_H__

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>  
#pragma comment(lib, "Ws2_32.lib") 

struct Packet {
	float x, y;
	BYTE stateMask;
};

enum class GAME_LEVEL : BYTE {
	EASY,
	NORMAL,
	HARD,
	NONE,
};

class PacketManager
{
public:
	PacketManager();
	~PacketManager();
	void Initialize(GAME_LEVEL level);
	void Reset();
	void SendPacket(BYTE flag, float x, float y);
	bool RecvPacket(Packet* packet);

	void SetSocketOpt();

	void SetIPAddress(char* ip);
	char* GetIPAddress() { return m_serverIP; };

	shared_ptr<queue<Packet>> GetPacketQueue();

	void WaitForPlayers();

private:

	SOCKET m_sock;
	shared_ptr<queue<Packet>> m_toClientEventQue;

<<<<<<< HEAD
    //임시로 사용할 ip, port, Bufsize
    //=======================================
    //char* SERVERIP = (char*)"192.168.204.231";
    char* SERVERIP = (char*)"127.0.0.1";
    int SERVERPORT = 9000;
    int BUFSIZE = 1024;
    //=======================================
=======
	char* m_serverIP = (char*)"127.0.0.1";

	//임시로 사용할 ip, port, Bufsize
	//=======================================
	char* SERVERIP = (char*)"127.0.0.1";
	int SERVERPORT = 9000;
	int BUFSIZE = 1024;
	//=======================================

public:
	array<Packet, 3> m_initPacket;

	static inline char* LPARAMToCharPtr(LPARAM lp)
	{
		in_addr addr;
		addr.s_addr = htonl(static_cast<u_long>(lp));
		return inet_ntoa(addr);
	}
>>>>>>> client/feature/startUI
};


#endif // !__PACKET_MANAGER_H__