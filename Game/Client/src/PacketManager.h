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
    NONE,
    EASY,
    NORMAL,
    HARD
};

class PacketManager
{

public:
    PacketManager(GAME_LEVEL level);
    ~PacketManager();
    void Initialize(GAME_LEVEL level);
    void SendPacket(Packet);
    bool RecvPacket();

    shared_ptr<queue<Packet>> GetPacketQueue();

private:

    SOCKET m_sock;
    shared_ptr<queue<Packet>> m_toClientEventQue;

    //임시로 사용할 ip, port, Bufsize
    //=======================================
    char* SERVERIP = (char*)"127.0.0.1";
    int SERVERPORT = 9000;
    int BUFSIZE = 1024;
    //=======================================
};


#endif // !__PACKET_MANAGER_H__