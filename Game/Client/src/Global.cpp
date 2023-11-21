#pragma once
#include "stdafx.h"
#include "Global.h"
#include "PacketManager.h"
PacketManager* g_PacketManager = nullptr;
g_prevTime = 0;
g_elapsedTime = 0;

class Global
{
public:
	Global()
	{
		g_PacketManager = new PacketManager();
		g_prevTime = std::chrono::system_clock::now();
	}
	~Global()
	{
		delete g_PacketManager;
	}
}GCoreGlobal;