#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"

#include "../features/chams/chams.h"
#include "../features/glow/glow.h"
#include "../features/backtrack/backtrack.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

static void AS_DPPSE_Callback()
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("DoPostScreenSpaceEffects", hooks))
		return;

	auto engine = API::GetScriptEngine();

	for (const auto& hook : hooks)
	{
		asIScriptContext* ctx = engine->RequestContext();

		ctx->Prepare(hook.func);
		ctx->Execute();

		engine->ReturnContext(ctx);
	}
}

DECLARE_VTABLE_HOOK(DoPostScreenSpaceEffects, bool, (IClientMode* thisptr, CViewSetup* setup))
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

	AS_DPPSE_Callback();
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