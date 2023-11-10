#include "stdafx.h"
#include "Global.h"
#include "PacketManager.h"
PacketManager* g_PacketManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		g_PacketManager = new PacketManager();
	}
	~CoreGlobal()
	{
		delete g_PacketManager;
	}
}GCoreGlobal;