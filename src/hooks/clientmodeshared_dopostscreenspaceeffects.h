#pragma once

#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"

#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(DoPostScreenSpaceEffects, bool, (IClientMode * thisptr, CViewSetup *setup))
{
	Backtrack::DoPostScreenSpaceEffects();
	Chams::Run();
	Glow::Run();

	if (Settings::Misc.no_zoom)
	{
		if (auto pLocal = EntityList::GetLocal(); pLocal != nullptr && pLocal->IsAlive())
		{
			if (pLocal->InCond(TF_COND_ZOOMED) && Thirdperson::IsThirdPerson(pLocal))
			{
				pLocal->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
				auto ent = pLocal->FirstShadowChild();
				while (ent != nullptr)
				{
					ent->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
					ent->NextShadowPeer();
				}
			}
		}
	}

	Hooks_CallHooks("DoPostScreenSpaceEffects");
	return originalDoPostScreenSpaceEffects(thisptr, setup);
}

inline void HookDoPostScreenSpaceEffects(void)
{
	INSTALL_VTABLE_HOOK(DoPostScreenSpaceEffects, interfaces::ClientMode, 40);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("ClientMode::DoPostScreenSpaceEffects Hooked\n", color);
#endif
}