#include "network.h"
#include "../../sdk/interfaces/interfaces.h"
#include "../entitylist/entitylist.h"

#include "../visuals/norecoil/norecoil.h"
#include "../aimbot/aimbot.h"
#include "../antiaim/antiaim.h"
#include "../bhop/bhop.h"
#include "../triggerbot/triggerbot.h"

#include "../lua/api.h"
#include "../lua/hookmgr.h"

bool g_bSendPacket = true;

void RunLuaCreateMoveCallback(CUserCmd* pCmd)
{
	lua_newtable(Lua::m_luaState);

	lua_pushstring(Lua::m_luaState, "buttons");
	lua_pushinteger(Lua::m_luaState, pCmd->buttons);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "tick_count");
	lua_pushinteger(Lua::m_luaState, pCmd->tick_count);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "forwardmove");
	lua_pushnumber(Lua::m_luaState, pCmd->forwardmove);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "sidemove");
	lua_pushnumber(Lua::m_luaState, pCmd->sidemove);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "upmove");
	lua_pushnumber(Lua::m_luaState, pCmd->upmove);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "mousedx");
	lua_pushinteger(Lua::m_luaState, pCmd->mousedx);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "mousedy");
	lua_pushinteger(Lua::m_luaState, pCmd->mousedy);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "weaponselect");
	lua_pushinteger(Lua::m_luaState, pCmd->weaponselect);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "weaponsubtype");
	lua_pushinteger(Lua::m_luaState, pCmd->weaponsubtype);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "impulse");
	lua_pushinteger(Lua::m_luaState, pCmd->impulse);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "hasbeenpredicted");
	lua_pushboolean(Lua::m_luaState, pCmd->hasbeenpredicted);
	lua_settable(Lua::m_luaState, -3);

	lua_pushstring(Lua::m_luaState, "sendpacket");
	lua_pushboolean(Lua::m_luaState, g_bSendPacket);
	lua_settable(Lua::m_luaState, -3);

	LuaHookManager::Call(Lua::m_luaState, "CreateMove", 1, false);

	lua_getfield(Lua::m_luaState, -1, "buttons");
	pCmd->buttons = lua_tointeger(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "tick_count");
	pCmd->tick_count = lua_tointeger(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "forwardmove");
	pCmd->forwardmove = lua_tonumber(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "sidemove");
	pCmd->sidemove = lua_tonumber(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "upmove");
	pCmd->upmove = lua_tonumber(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "mousedx");
	pCmd->mousedx = lua_tointeger(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "mousedy");
	pCmd->mousedy = lua_tointeger(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "weaponselect");
	pCmd->weaponselect = lua_tointeger(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "weaponsubtype");
	pCmd->weaponsubtype = lua_tointeger(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "impulse");
	pCmd->impulse = lua_tointeger(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "hasbeenpredicted");
	pCmd->hasbeenpredicted = lua_toboolean(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_getfield(Lua::m_luaState, -1, "sendpacket");
	g_bSendPacket = lua_toboolean(Lua::m_luaState, -1);
	lua_pop(Lua::m_luaState, 1);

	lua_pop(Lua::m_luaState, 1); // pop the table
}

void PostCreateMove(int sequence_number)
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

	Bhop::Run(pLocal, pCmd);
	Antiaim::Run(pLocal, pWeapon, pCmd);
	Aimbot::Run(pLocal, pWeapon, pCmd);
	Triggerbot::Run(pLocal, pWeapon, pCmd);

	if (LuaHookManager::HasHooks("CreateMove"))
		RunLuaCreateMoveCallback(pCmd);

	if (reinterpret_cast<CClientState*>(interfaces::ClientState)->chokedcommands >= 21)
		g_bSendPacket = true;

	if (g_bSendPacket)
		helper::localplayer::LastAngle = pCmd->viewangles;

	helper::engine::FixMovement(pCmd, originalAngles, pCmd->viewangles);
}