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

#include "../features/network/network.h"

#include "../features/entitylist/entitylist.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/antiaim/antiaim.h"
#include "../features/aimbot/aimbot.h"
#include "../features/bhop/bhop.h"
#include "../features/triggerbot/triggerbot.h"
#include "../features/warp/warp.h"

#include "../sdk/definitions/host.h"
#include "../sdk/definitions/protocol.h"

// Host_ShouldRun(void) 48 8B 15 ? ? ? ? B8 01 00 00 00 8B 72 58
// CL_Move(float accumulated_shit, bool bFinalTick) 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01
// CL_SendMove(void) 55 66 0F EF C0 48 89 E5 41 57 41 56 48 8D BD E8 EF FF FF
// func to get net_time CReplayServer::GetOnlineTime(CReplayServer*) 48 8D 05 ? ? ? ? 66 0F EF C9 F3 0F 5A 8F DC 93 00 00

// host_frametime_unbounded	F3 0F 10 00 48 8D 05 ?? ?? ?? ?? F3 0F 10 08 48 8B 07 48 8B 40 68 48 39 D0 0F 85 ?? ?? ?? ?? 8B 47 10
// host_frametime_stddeviation	F3 0F 10 08 48 8B 07 48 8B 40 68 48 39 D0 0F 85 ?? ?? ?? ?? 8B 47 10

using Host_ShouldRunFn = bool(*)(void);
inline Host_ShouldRunFn originalHost_ShouldRun = nullptr;

//DETOUR_DECL_TYPE(void, CL_Move, float accumulated_extra_samples, bool bFinalTick);
inline detour_ctx_t move_ctx;

inline void CL_SendMove( void )
{
	CClientState* cl = static_cast<CClientState*>(interfaces::ClientState);

	byte data[ 4000 ];

	int nextcommandnr = cl->lastoutgoingcommand + cl->chokedcommands + 1;

	// send the client update packet

	CLC_Move moveMsg;
	moveMsg.m_DataOut.StartWriting( data, sizeof( data ) );

	// How many real new commands have queued up	
	moveMsg.m_nNewCommands = 1 + cl->chokedcommands;
	moveMsg.m_nNewCommands = std::clamp( moveMsg.m_nNewCommands, 0, MAX_NEW_COMMANDS );

	// Determine number of backup commands to send along
	int extracmds = cl->chokedcommands + 1 - moveMsg.m_nNewCommands;
	int backupcmds = std::max(2, extracmds);
	moveMsg.m_nBackupCommands = std::clamp(0, backupcmds, MAX_BACKUP_COMMANDS);

	int numcmds = moveMsg.m_nNewCommands + moveMsg.m_nBackupCommands;
	int from = -1;	// first command is deltaed against zeros 

	bool bOK = true;

	for ( int to = nextcommandnr - numcmds + 1; to <= nextcommandnr; to++ )
	{
		bool isnewcmd = to >= (nextcommandnr - moveMsg.m_nNewCommands + 1);

		// first valid command number is 1
		bOK = bOK && interfaces::ClientDLL->WriteUsercmdDeltaToBuffer( &moveMsg.m_DataOut, from, to, isnewcmd );
		from = to;
	}

	if ( bOK )
	{
		if (extracmds)
			static_cast<CNetChannel*>(cl->m_NetChannel)->m_nChokedPackets -= extracmds;

		// only write message if all usercmds were written correctly, otherwise parsing would fail
		cl->m_NetChannel->SendNetMsg( moveMsg );
	}
}

inline void CL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	CClientState* cl = static_cast<CClientState*>(interfaces::ClientState);

	static ConVar* host_limitlocal = interfaces::Cvar->FindVar("host_limitlocal");
	static ConVar* cl_cmdrate = interfaces::Cvar->FindVar("cl_cmdrate");

	// returns the right thing
	static uintptr_t net_time_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 ? ? ? ? 66 0F EF C9 F3 0F 5A 8F DC 93 00 00"));
	double net_time = *reinterpret_cast<double*>(vtable::ResolveRIP(net_time_addr, 3, 7));

	// I'll make these sigs good later, I just need something that works right now
	static uintptr_t host_frametime_unbounded_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 C6 44 8B 00 F3 0F 10 08 48 8B 07 48 8B 40 68"));
	float host_frametime_unbounded = *reinterpret_cast<float*>(vtable::ResolveRIP(host_frametime_unbounded_addr, 3, 7));

	static uintptr_t host_frametime_stddeviation_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 DC 44 8B 00 48 8D 15 3D 41 00 00 49 8B BC 24 A8 00 00 00"));
	float host_frametime_stddeviation = *reinterpret_cast<float*>(vtable::ResolveRIP(host_frametime_stddeviation_addr, 3, 7));

	static uintptr_t host_state_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 ? ? ? ? 48 83 38 00 74 ? E9"));
	CCommonHostState* host_state = reinterpret_cast<CCommonHostState*>(vtable::ResolveRIP(host_state_addr, 3, 7));

	//interfaces::Cvar->ConsolePrintf("net_time: %f\n", net_time);

	if (!(cl->m_nSignonState >= SIGNONSTATE_CONNECTED))
		return;

	if (!originalHost_ShouldRun())
		return;

	g_bSendPacket = true;

	if (interfaces::Engine->IsPlayingDemo())
	{
		if (interfaces::Engine->IsHLTV())
		{
			g_bSendPacket = false;
		}
		else
		{
			return;
		}
	}

	//printf("net_time: %f, unbounded: %f, stddeviation: %f\n", net_time, host_frametime_unbounded, host_frametime_stddeviation);

	if  ( ( !cl->m_NetChannel->IsLoopback() || host_limitlocal->GetInt() ) &&
		 ( ( net_time < cl->m_flNextCmdTime ) || !cl->m_NetChannel->CanPacket()  || !bFinalTick ) )
	{
		g_bSendPacket = false;
	}

	if (cl->m_nSignonState == SIGNONSTATE_FULL)
	{
		int nextcommandnr = cl->lastoutgoingcommand + cl->chokedcommands + 1;

		interfaces::ClientDLL->CreateMove(nextcommandnr, host_state->interval_per_tick - accumulated_extra_samples, !cl->m_bPaused);

		// i dont think the game likes what im doing here
		// lol ts just crashes my game
		/*static uintptr_t demorecorder_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "4C 8D 3D 76 AF 6B 00 49 8B 3F 48 8B 07 FF 50 20 84 C0 74 0F 49 8B 3F"));
		uintptr_t demorecorder = vtable::ResolveRIP(demorecorder_addr, 3, 7);
		bool demorecorder_isrecording = demorecorder + 0x20;

		if (demorecorder_isrecording)
		{
			//using RecordInputFn = void(*)(void* self, int cmdnum);
			//reinterpret_cast<RecordInputFn>((demorecorder + 0x48))(reinterpret_cast<void*>(demorecorder), nextcommandnr);
			//vtable::call<10, int>(demorecorder, nextcommandnr);
		}*/

		PostCreateMove(nextcommandnr);

		if (g_bSendPacket)
		{
			CL_SendMove();
		}
		else
		{
			cl->m_NetChannel->SetChoked();
			cl->chokedcommands++;
		}
	}

	if (!g_bSendPacket)
		return;

	bool hasProblem = cl->m_NetChannel->IsTimingOut() && cl->m_nSignonState == SIGNONSTATE_FULL;
	if (hasProblem && cl->m_nDeltaTick != -1)
		cl->m_nDeltaTick = -1;

	if (cl->m_nSignonState == SIGNONSTATE_FULL)
	{
		//interfaces::Cvar->ConsolePrintf("CL_Move - delta tick: %i, unbounded: %f, deviation: %f\n", cl->m_nDeltaTick, host_frametime_unbounded, host_frametime_stddeviation);
		NET_Tick mymsg(cl->m_nDeltaTick, host_frametime_unbounded, host_frametime_stddeviation);
		cl->m_NetChannel->SendNetMsg(mymsg);
	}

	cl->lastoutgoingcommand = cl->m_NetChannel->SendDatagram(NULL);
	cl->chokedcommands = 0;

	if (cl->m_nSignonState == SIGNONSTATE_FULL)
	{
		float commandInterval = 1.0f / cl_cmdrate->GetFloat();
		float maxDelta = std::min ( host_state->interval_per_tick, commandInterval );
		float delta = std::clamp( (float)(net_time - cl->m_flNextCmdTime), 0.0f, maxDelta );
		cl->m_flNextCmdTime = net_time + commandInterval - delta;
	}
	else
	{
		cl->m_flNextCmdTime = net_time + (1.0f/5.0f);
	}
}

inline void HookedCL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	Warp::m_bShifting = false;
	Warp::m_bRecharging = false;

	if (Warp::m_iDesiredState == WarpState::RECHARGING && Warp::m_iStoredTicks < Warp::GetMaxTicks())
	{
		Warp::m_iStoredTicks++;
		Warp::m_bRecharging = true;
		return;	
	}

	CL_Move(accumulated_extra_samples, bFinalTick);

	if (Warp::m_iDesiredState == WarpState::RUNNING && Warp::m_iStoredTicks > 0)
	{
		Warp::m_bShifting = true;
		Warp::m_iShiftAmount = Settings::antiaim.warp_speed + 1;

		for (int n = 0; n < (Settings::antiaim.warp_speed + 1); n++)
		{
			if (Warp::m_iStoredTicks <= 0)
				break;

			CL_Move(accumulated_extra_samples, n == Settings::antiaim.warp_speed);
			Warp::m_iStoredTicks--;
		}

		Warp::m_iDesiredState = WarpState::WAITING;
		Warp::m_bShifting = false;
		return;
	}

	if (Warp::m_iDesiredState == WarpState::DT && Warp::m_iStoredTicks >= Warp::GetMaxTicks())
	{
		const int ticks = Warp::m_iStoredTicks;

		Warp::m_bShifting = true;
		Warp::m_iShiftAmount = ticks;

		for (int n = 0; n < ticks; n++)
			CL_Move(accumulated_extra_samples, n == ticks - 1);
		
		Warp::m_iStoredTicks = 0;
		Warp::m_iDesiredState = WarpState::WAITING;
		Warp::m_bShifting = false;
		return;
	}
}

inline void HookCL_Move(void)
{
	originalHost_ShouldRun = reinterpret_cast<Host_ShouldRunFn>(sigscan_module("engine.so", "48 8B 15 ? ? ? ? B8 01 00 00 00 8B 72 58"));

	void* original_Move = sigscan_module("engine.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01");
	detour_init(&move_ctx, original_Move, (void*)&HookedCL_Move);
	
	if (!detour_enable(&move_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Couldn't hook CL_Move\n");
		return;
	}

	interfaces::Cvar->ConsolePrintf("CL_Move hooked\n");
}
