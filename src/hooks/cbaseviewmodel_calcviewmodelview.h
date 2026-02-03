#pragma once

#include "../tracy/tracy/Tracy.hpp"

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"
#include <string>

#include "../features/aimbot/aimbot.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_offset/viewmodel_offset.h"

#include "../libdetour/libdetour.h"

#include "../features/lua/hooks.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"

using CalcViewModelViewFn = void(*)(void* thisptr, CBaseEntity* owner, const Vector& eyePosition, const QAngle& eyeAngles);
inline CalcViewModelViewFn originalCalcViewModelView = nullptr;

DETOUR_DECL_TYPE(void, originalCalcViewModelView, void* thisptr, CBaseEntity*, const Vector&, const QAngle&);
inline detour_ctx_t calcViewModel_ctx;

inline void HookedCalcViewModelView(void* thisptr, CBaseEntity* owner, const Vector& eyePosition, const QAngle& eyeAngles)
{
	ZoneScoped;

	Vector angle = eyeAngles;
	Vector position = eyePosition;

	if (owner)
	{
		if (LuaHookManager::HasHooks("CalcViewModelView"))
		{
			LuaClasses::VectorLua::push_vector(Lua::m_luaState, position);
			LuaClasses::VectorLua::push_vector(Lua::m_luaState, angle);

			LuaHookManager::Call(Lua::m_luaState, "CalcViewModelView", 2);
		}

		g_ViewmodelInterp.Run(angle);
		g_ViewmodelAim.Run(angle);
		g_ViewmodelOffset.Run(position, angle);
	}

	DETOUR_ORIG_CALL(&calcViewModel_ctx, originalCalcViewModelView, thisptr, owner, position, angle);
}

inline void HookCalcViewModelView()
{
	originalCalcViewModelView = (CalcViewModelViewFn)(sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 48 83 EC 48 8B 41 08"));
	detour_init(&calcViewModel_ctx, (void*)originalCalcViewModelView, (void*)&HookedCalcViewModelView);
	detour_enable(&calcViewModel_ctx);

	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CalcViewModelView hooked\n");
}