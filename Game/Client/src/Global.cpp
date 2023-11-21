#pragma once
#include "stdafx.h"
#include "Global.h"
#include "PacketManager.h"
PacketManager* g_PacketManager = nullptr;
std::chrono::system_clock::time_point g_prevTime = std::chrono::system_clock::now();
double g_elapsedTime = 0.f;

class Global
{
public:
	Global()
	{
		g_PacketManager = new PacketManager();
	}
	~Global()
	{
		delete g_PacketManager;
	}
}GCoreGlobal;