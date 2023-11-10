#pragma once
#include "stdafx.h"
#include "Global.h"
#include "PacketManager.h"
PacketManager* g_PacketManager = nullptr;

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