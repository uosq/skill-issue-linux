#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../features/entitylist/entitylist.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings/settings.h"
#include <string>

#include "../features/lua/classes.h"
#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"

#include "../features/visuals/customfov/customfov.h"
#include "../features/visuals/norecoil/norecoil.h"

DECLARE_VTABLE_HOOK(OverrideView, void, (IClientMode *thisptr, CViewSetup *pView))
{
	originalOverrideView(thisptr, pView);

	if (pView == nullptr)
		return;

	if (LuaHookManager::HasHooks("OverrideView"))
	{
		LuaClasses::ViewSetupLua::push_viewsetup(Lua::m_luaState, pView);
		LuaHookManager::Call(Lua::m_luaState, "OverrideView", 1);
	}

	if (CTFPlayer* pLocal = EntityList::GetLocal(); pLocal != nullptr)
	{
		if (!pLocal->IsAlive())
			return;

		NoRecoil::RunOverrideView(pLocal, pView);
		CustomFov::Run(pLocal, pView);
	}
}

inline void HookOverrideView()
{
	INSTALL_VTABLE_HOOK(OverrideView, interfaces::ClientMode, 17);

	#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("ClientModeShared::OverrideView hooked\n", color);
	#endif
}