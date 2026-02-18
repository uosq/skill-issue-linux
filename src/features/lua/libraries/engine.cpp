#include "../classes.h"
#include "../libraries.h"
#include "../api.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"

namespace LuaFuncs
{
	namespace engine
	{
		const luaL_Reg enginelib[] =
		{
			{"IsInGame", IsInGame},
			{"IsConnected", IsConnected},
			{"IsTakingScreenshot", IsTakingScreenshot},
			{"IsGameUIVisible", IsGameUIVisible},
			{"IsConsoleVisible", IsConsoleVisible},
			{"GetViewAngles", GetViewAngles},
			{"SetViewAngles", SetViewAngles},
			{"GetNetChannel", GetNetChannel},
			{"Trace", Trace},
			{"TraceHull", TraceHull},
			{"GetPointContents", GetPointContent},
			{"WorldToScreen", WorldToScreen},
			{nullptr, nullptr},
		};

		void luaopen_engine(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, enginelib, 0);
			lua_setglobal(L, "engine");

			//consoleText += "engine library initialized\n";
		}

		int IsInGame(lua_State* L)
		{
			lua_pushboolean(L, interfaces::Engine->IsInGame());
			return 1;
		}

		int IsConnected(lua_State* L)
		{
			lua_pushboolean(L, interfaces::Engine->IsConnected());
			return 1;
		}

		int IsTakingScreenshot(lua_State* L)
		{
			lua_pushboolean(L, interfaces::Engine->IsTakingScreenshot());
			return 1;
		}

		int IsGameUIVisible(lua_State* L)
		{
			lua_pushboolean(L, interfaces::EngineVGui->IsGameUIVisible());
			return 1;
		}
		
		int IsConsoleVisible(lua_State* L)
		{
			lua_pushboolean(L, interfaces::Engine->Con_IsVisible());
			return 1;
		}

		int GetViewAngles(lua_State* L)
		{
			Vector angles;
			interfaces::Engine->GetViewAngles(angles);
			LuaClasses::Vector3::push(L, angles);
			return 1;
		}

		int SetViewAngles(lua_State* L)
		{
			LuaVector* vec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
			if (vec == nullptr)
				return 0;

			interfaces::Engine->SetViewAngles(*vec);
			return 0;
		}

		int WorldToScreen(lua_State* L)
		{
			LuaVector* vec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
			if (vec == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			// never let it be from some random shit in the stack
			Vector out = {0, 0, 0};
			if (!helper::engine::WorldToScreen(*vec, out))
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::Vector3::push(L, out);
			return 1;
		}

		int GetNetChannel(lua_State* L)
		{
			CNetChannel* netchan = reinterpret_cast<CNetChannel*>(interfaces::Engine->GetNetChannelInfo());
			if (netchan == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::NetChannelLua::push_netchannel(Lua::m_luaState, netchan);
			return 1;
		}

		// engine.Trace(start, end, mask, shouldHitFunction(entity, contentsmask))
		int Trace(lua_State* L)
		{
			LuaVector* start = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
			if (!start)
				return 0;

			LuaVector* end = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));
			if (!end)
				return 0;

			int mask = luaL_optinteger(L, 3, 0);

			Ray_t ray;
			ray.Init(*start, *end);

			CTraceFilterLua filter;
			CGameTrace trace;
			interfaces::EngineTrace->TraceRay(ray, mask, &filter, &trace);

			// warning:
			// big ass table below!
			lua_newtable(L);

			lua_pushinteger(L, trace.hitbox);
			lua_setfield(L, -2, "hitbox");

			lua_pushinteger(L, trace.hitgroup);
			lua_setfield(L, -2, "hitgroup");

			lua_pushnumber(L, trace.fractionleftsolid);
			lua_setfield(L, -2, "fractionleftsolid");

			lua_pushinteger(L, trace.contents);
			lua_setfield(L, -2, "contents");

			lua_pushboolean(L, trace.allsolid);
			lua_setfield(L, -2, "allsolid");

			lua_pushnumber(L, trace.fraction);
			lua_setfield(L, -2, "fraction");

			LuaClasses::Vector3::push(L, trace.plane.normal);
			lua_setfield(L, -2, "plane");

			if (trace.m_pEnt != nullptr)
			{
				LuaClasses::EntityLua::push_entity(L, trace.m_pEnt);
				lua_setfield(L, -2, "entity");
			}

			LuaClasses::Vector3::push(L, trace.startpos);
			lua_setfield(L, -2, "startpos");

			LuaClasses::Vector3::push(L, trace.endpos);
			lua_setfield(L, -2, "endpos");

			lua_pushinteger(L, trace.surface.surfaceProps);
			lua_setfield(L, -2, "surfaceProps");

			lua_pushinteger(L, trace.surface.flags);
			lua_setfield(L, -2, "surfaceFlags");

			lua_pushstring(L, trace.surface.name);
			lua_setfield(L, -2, "surfaceName");

			lua_pushinteger(L, trace.dispFlags);
			lua_setfield(L, -2, "dispFlags");

			lua_pushboolean(L, trace.startsolid);
			lua_setfield(L, -2, "startsolid");

			lua_pushboolean(L, trace.IsDispSurface());
			lua_setfield(L, -2, "IsDistSurface");

			lua_pushboolean(L, trace.IsDispSurfaceBuildable());
			lua_setfield(L, -2, "IsDistSurfaceBuildable");

			lua_pushboolean(L, trace.IsDispSurfaceProp1());
			lua_setfield(L, -2, "IsDistSurfaceProp1");

			lua_pushboolean(L, trace.IsDispSurfaceProp2());
			lua_setfield(L, -2, "IsDistSurfaceProp2");

			lua_pushboolean(L, trace.IsDispSurfaceWalkable());
			lua_setfield(L, -2, "IsDistSurfaceWalkable");

			return 1;
		}

		int TraceHull(lua_State* L)
		{
			LuaVector* start = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
			if (!start)
				return 0;

			LuaVector* end = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));
			if (!end)
				return 0;

			LuaVector* min = static_cast<LuaVector*>(luaL_checkudata(L, 3, "Vector3"));
			if (!min)
				return 0;

			LuaVector* max = static_cast<LuaVector*>(luaL_checkudata(L, 4, "Vector3"));
			if (!max)
				return 0;

			int mask = luaL_optinteger(L, 5, 0);

			Ray_t ray;
			ray.Init(*start, *end, *min, *max);

			CTraceFilterLua filter;
			CGameTrace trace;
			interfaces::EngineTrace->TraceRay(ray, mask, &filter, &trace);

			// warning:
			// big ass table below!
			lua_newtable(L);

			lua_pushinteger(L, trace.hitbox);
			lua_setfield(L, -2, "hitbox");

			lua_pushinteger(L, trace.hitgroup);
			lua_setfield(L, -2, "hitgroup");

			lua_pushnumber(L, trace.fractionleftsolid);
			lua_setfield(L, -2, "fractionleftsolid");

			lua_pushinteger(L, trace.contents);
			lua_setfield(L, -2, "contents");

			lua_pushboolean(L, trace.allsolid);
			lua_setfield(L, -2, "allsolid");

			lua_pushnumber(L, trace.fraction);
			lua_setfield(L, -2, "fraction");

			LuaClasses::Vector3::push(L, trace.plane.normal);
			lua_setfield(L, -2, "plane");

			if (trace.m_pEnt != nullptr)
			{
				LuaClasses::EntityLua::push_entity(L, trace.m_pEnt);
				lua_setfield(L, -2, "entity");
			}

			LuaClasses::Vector3::push(L, trace.startpos);
			lua_setfield(L, -2, "startpos");

			LuaClasses::Vector3::push(L, trace.endpos);
			lua_setfield(L, -2, "endpos");

			lua_pushinteger(L, trace.surface.surfaceProps);
			lua_setfield(L, -2, "surfaceProps");

			lua_pushinteger(L, trace.surface.flags);
			lua_setfield(L, -2, "surfaceFlags");

			lua_pushstring(L, trace.surface.name);
			lua_setfield(L, -2, "surfaceName");

			lua_pushinteger(L, trace.dispFlags);
			lua_setfield(L, -2, "dispFlags");

			lua_pushboolean(L, trace.startsolid);
			lua_setfield(L, -2, "startsolid");

			lua_pushboolean(L, trace.IsDispSurface());
			lua_setfield(L, -2, "IsDistSurface");

			lua_pushboolean(L, trace.IsDispSurfaceBuildable());
			lua_setfield(L, -2, "IsDistSurfaceBuildable");

			lua_pushboolean(L, trace.IsDispSurfaceProp1());
			lua_setfield(L, -2, "IsDistSurfaceProp1");

			lua_pushboolean(L, trace.IsDispSurfaceProp2());
			lua_setfield(L, -2, "IsDistSurfaceProp2");

			lua_pushboolean(L, trace.IsDispSurfaceWalkable());
			lua_setfield(L, -2, "IsDistSurfaceWalkable");

			return 1;
		}

		int GetPointContent(lua_State* L)
		{
			LuaVector* vec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
			if (!vec)
				return 0;

			lua_pushinteger(L, interfaces::EngineTrace->GetPointContents(*vec));
			return 1;
		}
	}
}