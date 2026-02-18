#pragma once

#include "../../../sdk/definitions/cusercmd.h"
#include "../pluto/lua.h"
#include <cstdint>

struct LuaUserCmd
{
	int command_number;
	int tick_count;
	QAngle viewangles;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	uint8_t impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	int server_random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
	bool sendpacket;
	bool valid;

	// C methods
	void CopyFromUserCmd(const CUserCmd& pCmd);
	void WriteToUserCmd(CUserCmd* pOut);

	// Lua methods
	static int __index(lua_State* L);
	static int __newindex(lua_State* L);
};

namespace LuaClasses
{
	namespace UserCmd
	{
		LuaUserCmd* push(lua_State* L, const CUserCmd& pCmd);
		void open(lua_State* L);
	}
}