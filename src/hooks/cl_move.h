#pragma once

#include "../sdk/definitions/igameevents.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"
#include "../libdetour/libdetour.h"
#include "../sdk/definitions/inetchannel.h"

#include "../sdk/definitions/cclientstate.h"

#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"

// Host_ShouldRun(void) 48 8B 15 ? ? ? ? B8 01 00 00 00 8B 72 58
// CL_Move(float accumulated_shit, bool bFinalTick) 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01
// CL_SendMove(void) 55 66 0F EF C0 48 89 E5 41 57 41 56 48 8D BD E8 EF FF FF
// func to get net_time CReplayServer::GetOnlineTime(CReplayServer*) 48 8D 05 ? ? ? ? 66 0F EF C9 F3 0F 5A 8F DC 93 00 00

using CL_MoveFn = void(*)(float accumulated_extra_samples, bool bFinalTick);
inline CL_MoveFn originalCL_Move = nullptr;

using Host_ShouldRunFn = bool(*)(void);
inline Host_ShouldRunFn originalHost_ShouldRun = nullptr;

using CL_SendMoveFn = void(*)(void);
inline CL_SendMoveFn originalCL_SendMove = nullptr;

using GetOnlineTimeFn = double(*)(void*);
inline double* g_NetTime = nullptr;

inline bool g_bSendPacket = true;

// My rebuild is absolute garbage
// TODO: Finish this some other day
inline void HookedCL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	if (!interfaces::Engine->IsConnected())
		return;

	if (!originalHost_ShouldRun())
		return;

	if (interfaces::Engine->IsPlayingDemo())
	{
		if (interfaces::Engine->IsHLTV()) g_bSendPacket = false;
		else return;
	}

	static ConVar* host_limitlocal = interfaces::Cvar->FindVar("host_limitlocal");
	static ConVar* host_frametime_unbounded = interfaces::Cvar->FindVar("host_frametime_unbounded");
	static ConVar* host_frametime_stddeviation = interfaces::Cvar->FindVar("host_frametime_stddeviation");
	CClientState* cl = static_cast<CClientState*>(interfaces::ClientState);

	if ((cl->m_NetChannel->IsLoopback() || host_limitlocal->GetInt()) &&
		(!cl->m_NetChannel->CanPacket() || *g_NetTime < cl->m_flNextCmdTime || !bFinalTick))
	{
		g_bSendPacket = false;
	}

	if (!cl->m_bPaused)
	{
		int nextcommandnr = cl->lastoutgoingcommand + cl->chokedcommands + 1;

		interfaces::ClientDLL->CreateMove(nextcommandnr, interfaces::GlobalVars->interval_per_tick - accumulated_extra_samples, !cl->m_bPaused);
	}

	if (g_bSendPacket)
		originalCL_SendMove();
	else
	{
		cl->m_NetChannel->SetChoked();
		cl->chokedcommands++;
	}

	if (!g_bSendPacket)
		return;

	bool hasProblem = cl->m_NetChannel->IsTimedOut() && !cl->m_bPaused;

	if (hasProblem)
	{	// TODO: DO THIS SHIT RIGHT
		// man I never expected to use that here xddd
		Lua::RunCode("clientstate.ForceFullUpdate()");
	}

	if (!cl->m_bPaused)
	{
		NET_Tick mymsg(cl->m_nDeltaTick, host_frametime_unbounded->GetFloat(), host_frametime_stddeviation->GetFloat());
		cl->m_NetChannel->SendNetMsg(mymsg);
	}

	originalCL_Move(accumulated_extra_samples, bFinalTick);
}

inline void HookCL_Move(void)
{
	GetOnlineTimeFn func = reinterpret_cast<GetOnlineTimeFn>(sigscan_module("engine.so", "48 8D 05 ? ? ? ? 66 0F EF C9 F3 0F 5A 8F DC 93 00 00"));
	g_NetTime = reinterpret_cast<double*>(vtable::ResolveRIP(reinterpret_cast<uintptr_t>(func), 3, 7));
}
