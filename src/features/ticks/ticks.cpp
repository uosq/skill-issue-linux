#include "ticks.h"
#include "../entitylist/entitylist.h"

#include "../visuals/norecoil/norecoil.h"
#include "../aimbot/aimbot.h"
#include "../antiaim/antiaim.h"
#include "../bhop/bhop.h"
#include "../triggerbot/triggerbot.h"
#include "../warp/warp.h"
#include "../fakelag/fakelag.h"

#include "../lua/api.h"
#include "../lua/hookmgr.h"
#include "../lua/classes/usercmdlua.h"

#include "../../sdk/definitions/host.h"
#include "../../sdk/definitions/protocol.h"
#include "../../sdk/definitions/con_nprint.h"

#include "../../sdk/helpers/convars/convars.h"

// Host_ShouldRun(void) 48 8B 15 ? ? ? ? B8 01 00 00 00 8B 72 58
// func to get net_time CReplayServer::GetOnlineTime(CReplayServer*) 48 8D 05 ? ? ? ? 66 0F EF C9 F3 0F 5A 8F DC 93 00 00

// host_frametime_unbounded	F3 0F 10 00 48 8D 05 ?? ?? ?? ?? F3 0F 10 08 48 8B 07 48 8B 40 68 48 39 D0 0F 85 ?? ?? ?? ?? 8B 47 10
// host_frametime_stddeviation	F3 0F 10 08 48 8B 07 48 8B 40 68 48 39 D0 0F 85 ?? ?? ?? ?? 8B 47 10

using Host_ShouldRunFn = bool(*)(void);
static Host_ShouldRunFn originalHost_ShouldRun = nullptr;

bool TickManager::m_bSendPacket = true;
uint8_t TickManager::m_iChokedCommands = 0;

void TickManager::Lua_CreateMove_Callback(CUserCmd* pCmd)
{
	if (!LuaHookManager::HasHooks("CreateMove"))
		return;

	LuaUserCmd* lcmd = LuaClasses::UserCmd::push(Lua::m_luaState, *pCmd);

	LuaHookManager::Call(Lua::m_luaState, "CreateMove", 1, false);
	lcmd->WriteToUserCmd(pCmd);
	lcmd->valid = false;

	lua_pop(Lua::m_luaState, 1);
}

void TickManager::Post_CreateMove(int sequence_number)
{
	CUserCmd* pCmd = &(static_cast<IInput*>(interfaces::CInput))->m_pCommands[sequence_number % 90];
	if (pCmd == nullptr)
		return;

	if (!interfaces::Engine->IsInGame() || !interfaces::Engine->IsConnected() || interfaces::Engine->IsPlayingDemo())
		return;

	EntityList::m_pAimbotTarget = nullptr;

	CTFPlayer* pLocal = EntityList::GetLocal();
	if (!pLocal || !pLocal->IsAlive() || pLocal->IsTaunting())
		return;

	CTFWeaponBase* pWeapon = HandleAs<CTFWeaponBase*>(pLocal->GetActiveWeapon());
	if (pWeapon == nullptr)
		return;

	Vector originalAngles = pCmd->viewangles;

	NoRecoil::RunCreateMove(pLocal, pWeapon, pCmd);

	FakeLag::Run();

	Bhop::Run(pLocal, pCmd);
	Antiaim::Run(pLocal, pWeapon, pCmd);
	Aimbot::Run(pLocal, pWeapon, pCmd);
	Triggerbot::Run(pLocal, pWeapon, pCmd);

	Lua_CreateMove_Callback(pCmd);

	if (reinterpret_cast<CClientState*>(interfaces::ClientState)->chokedcommands >= 21)
		m_bSendPacket = true;

	if (m_bSendPacket)
		helper::localplayer::LastAngle = pCmd->viewangles;

	Warp::RunCreateMove(pLocal, pWeapon, pCmd);

	helper::engine::FixMovement(pCmd, originalAngles, pCmd->viewangles);
}

void TickManager::CL_SendMove(void)
{
	CClientState* cl = static_cast<CClientState*>(interfaces::ClientState);

	byte data[ 4000 ];

	int nextcommandnr = cl->lastoutgoingcommand + m_iChokedCommands + 1;

	// send the client update packet

	CLC_Move moveMsg;
	moveMsg.m_DataOut.StartWriting( data, sizeof( data ) );

	// How many real new commands have queued up	
	moveMsg.m_nNewCommands = 1 + m_iChokedCommands;
	moveMsg.m_nNewCommands = std::clamp( moveMsg.m_nNewCommands, 0, MAX_NEW_COMMANDS );

	// Determine number of backup commands to send along
	int extracmds = m_iChokedCommands + 1 - moveMsg.m_nNewCommands;
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

void TickManager::CL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	CClientState* cl = static_cast<CClientState*>(interfaces::ClientState);

	// returns the right thing
	static uintptr_t net_time_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 ? ? ? ? 66 0F EF C9 F3 0F 5A 8F DC 93 00 00"));
	double net_time = *reinterpret_cast<double*>(vtable::ResolveRIP(net_time_addr, 3, 7));

	// I'll make these sigs good later, I just need something that works right now
	static uintptr_t host_frametime_unbounded_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 ? ? ? ? F3 0F 10 08 48 8B 07 48 8B 40 ?"));
	float host_frametime_unbounded = *reinterpret_cast<float*>(vtable::ResolveRIP(host_frametime_unbounded_addr, 3, 7));

	static uintptr_t host_frametime_stddeviation_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 ? ? ? ? 48 8D 15 ? ? ? ? 49 8B BC 24 ? ? ? ?"));
	float host_frametime_stddeviation = *reinterpret_cast<float*>(vtable::ResolveRIP(host_frametime_stddeviation_addr, 3, 7));

	static uintptr_t host_state_addr = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "48 8D 05 ? ? ? ? 48 83 38 00 74 ? E9"));
	CCommonHostState* host_state = reinterpret_cast<CCommonHostState*>(vtable::ResolveRIP(host_state_addr, 3, 7));

	//interfaces::Cvar->ConsolePrintf("net_time: %f\n", net_time);

	if (!(cl->m_nSignonState >= SIGNONSTATE_CONNECTED))
		return;

	if (!originalHost_ShouldRun())
		return;

	m_bSendPacket = true;

	if (interfaces::Engine->IsPlayingDemo())
	{
		if (interfaces::Engine->IsHLTV())
		{
			m_bSendPacket = false;
		}
		else
		{
			return;
		}
	}

	//printf("net_time: %f, unbounded: %f, stddeviation: %f\n", net_time, host_frametime_unbounded, host_frametime_stddeviation);

	if  ( ( !cl->m_NetChannel->IsLoopback() || ConVars::host_limitlocal->GetInt() ) &&
		 ( ( net_time < cl->m_flNextCmdTime ) || !cl->m_NetChannel->CanPacket()  || !bFinalTick ) )
	{
		m_bSendPacket = false;
	}

	if (cl->m_nSignonState == SIGNONSTATE_FULL)
	{
		int nextcommandnr = cl->lastoutgoingcommand + m_iChokedCommands + 1;

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

		Post_CreateMove(nextcommandnr);

		if (m_bSendPacket)
		{
			CL_SendMove();
		}
		else
		{
			cl->m_NetChannel->SetChoked();
			cl->chokedcommands++;
			m_iChokedCommands++;
		}
	}

	if (!m_bSendPacket)
		return;

	bool hasProblem = cl->m_NetChannel->IsTimingOut() && cl->m_nSignonState == SIGNONSTATE_FULL;
	if (hasProblem && cl->m_nDeltaTick != -1)
	{
		using Con_NXPrintf = void(*)(const con_nprint_t*, const char* fmt, ...);
		static auto original = reinterpret_cast<Con_NXPrintf>(sigscan_module("engine.so", "55 49 89 F2 48 89 E5 41 55 41 54 49 89 FC 48 81 EC D0 10 00 00"));

		con_nprint_t np;
		np.time_to_live = 1.0;
		np.index = 2;
		np.fixed_width_font = false;
		np.color[0] = 1.0;
		np.color[1] = 0.2;
		np.color[2] = 0.2;

		float flTimeOut = cl->m_NetChannel->GetTimeoutSeconds();
		assert(flTimeOut != -1.0f);
		float flRemainingTime = flTimeOut - cl->m_NetChannel->GetTimeSinceLastReceived();

		original( &np, "WARNING:  Connection Problem" );

		np.index = 3;
		original( &np, "Auto-disconnect in %.1f seconds", flRemainingTime );

		cl->m_nDeltaTick = -1;
	}

	if (cl->m_nSignonState == SIGNONSTATE_FULL)
	{
		//interfaces::Cvar->ConsolePrintf("CL_Move - delta tick: %i, unbounded: %f, deviation: %f\n", cl->m_nDeltaTick, host_frametime_unbounded, host_frametime_stddeviation);
		NET_Tick mymsg(cl->m_nDeltaTick, host_frametime_unbounded, host_frametime_stddeviation);
		cl->m_NetChannel->SendNetMsg(mymsg);
	}

	cl->lastoutgoingcommand = cl->m_NetChannel->SendDatagram(NULL);
	cl->chokedcommands = 0;
	m_iChokedCommands = 0;

	if (cl->m_nSignonState == SIGNONSTATE_FULL)
	{
		const float commandInterval = 1.0f / ConVars::cl_cmdrate->GetFloat();
		const float maxDelta = std::min ( host_state->interval_per_tick, commandInterval );
		const float delta = std::clamp( static_cast<float>(net_time - cl->m_flNextCmdTime), 0.0f, maxDelta );
		cl->m_flNextCmdTime = net_time + commandInterval - delta;
	}
	else
	{
		cl->m_flNextCmdTime = net_time + (1.0f/5.0f);
	}
}

void TickManager::Init()
{
	originalHost_ShouldRun = reinterpret_cast<Host_ShouldRunFn>(sigscan_module("engine.so", "48 8B 15 ? ? ? ? B8 01 00 00 00 8B 72 58"));
	m_bSendPacket = true; // just in case yk
	m_iChokedCommands = 0;
}

void TickManager::Run(float accumulated_extra_samples, bool bFinalTick)
{
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
		Warp::m_iShiftAmount = Settings::AntiAim::warp_speed;

		for (int n = 0; n < (Settings::AntiAim::warp_speed); n++)
		{
			if (Warp::m_iStoredTicks <= 0)
				break;

			CL_Move(accumulated_extra_samples, n == Settings::AntiAim::warp_speed);
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
		Warp::m_bDoubleTap = true;
		Warp::m_iShiftAmount = ticks;

		for (int n = 0; n < ticks; n++)
			CL_Move(accumulated_extra_samples, n == ticks - 1);
		
		Warp::m_iStoredTicks = 0;
		Warp::m_iDesiredState = WarpState::WAITING;
		Warp::m_bShifting = false;
		Warp::m_bDoubleTap = false;
		return;
	}
}