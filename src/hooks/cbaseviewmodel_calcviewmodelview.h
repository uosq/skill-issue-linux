#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"
#include <string>

#include "../features/aimbot/aimbot.h"

#include "../libdetour/libdetour.h"

#include "../features/lua/hooks.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"

using CalcViewModelViewFn = void(*)(void* thisptr, CBaseEntity* owner, const Vector& eyePosition, const QAngle& eyeAngles);
inline CalcViewModelViewFn originalCalcViewModelView = nullptr;

DETOUR_DECL_TYPE(void, originalCalcViewModelView, void* thisptr, CBaseEntity*, const Vector&, const QAngle&);

#define VIEWMODELAIM_INTERVAL 0.5f
inline static float stoptime = 0.0f;
inline detour_ctx_t calcViewModel_ctx;

inline void HookedCalcViewModelView(void* thisptr, CBaseEntity* owner, const Vector& eyePosition, const QAngle& eyeAngles)
{
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

		if (g_Settings.misc.viewmodel_interp > 0.0f)
		{
			if (auto pLocal = helper::engine::GetLocalPlayer(); pLocal != nullptr)
			{
				if (pLocal->IsAlive())
				{
					if (auto pWeapon = HandleAs<CTFWeaponBase*>(pLocal->GetActiveWeapon()); pWeapon != nullptr)
					{
						static Vector smooth;
						float frameTime = interfaces::GlobalVars->absolute_frametime;

						float alpha = 1.0f - expf(-g_Settings.misc.viewmodel_interp * frameTime);

						smooth.x += Math::NormalizeAngle(angle.x - smooth.x) * alpha;
						smooth.z += Math::NormalizeAngle(angle.z - smooth.z) * alpha;
						smooth.y += Math::NormalizeAngle(angle.y - smooth.y) * alpha;

						angle = smooth;
					}
				}
			}
		}

		if (g_Settings.aimbot.viewmodelaim)
		{
			if (Aimbot::IsRunning() && interfaces::GlobalVars && interfaces::GlobalVars->curtime)
				stoptime = interfaces::GlobalVars->curtime + VIEWMODELAIM_INTERVAL;
		
			if (interfaces::GlobalVars && interfaces::GlobalVars->curtime && interfaces::GlobalVars->curtime < stoptime)
				angle = Aimbot::GetAngle();
		}

		Vector offset = {g_Settings.misc.viewmodel_offset[0], g_Settings.misc.viewmodel_offset[1], g_Settings.misc.viewmodel_offset[2]};
		if (!offset.IsZero())
		{
			Vector forward, right, up;
			Math::AngleVectors(angle, &forward, &right, &up);

			position += forward * offset.x;
			position += right * offset.y;
			position += up * offset.z;
		}
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