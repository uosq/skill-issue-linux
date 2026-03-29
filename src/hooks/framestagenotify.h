#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/helpers/helper.h"
#include "../features/visuals/thirdperson/thirdperson.h"
#include "../features/visuals/customfov/customfov.h"
#include "../features/entitylist/entitylist.h"

#if 0
#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#endif

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

static void AS_FrameStage_Callback(int stage)
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("FrameStageNotify", hooks))
		return;

	auto engine = API::GetScriptEngine();

	for (const auto& hook : hooks)
	{
		asIScriptContext* ctx = engine->RequestContext();

		ctx->Prepare(hook.func);
		ctx->SetArgDWord(0, stage);
		ctx->Execute();

		engine->ReturnContext(ctx);
	}
}

DECLARE_VTABLE_HOOK(FrameStageNotify, void, (CHLClient* thisptr, int stage))
{
	switch (stage)
	{
		case FRAME_RENDER_START:
		{
			if (Settings::Misc.thirdperson_key->IsActive())
			{
				CTFPlayer* pLocal = EntityList::GetLocal();
				if (pLocal == nullptr || !pLocal->IsAlive())
					break;
				
				interfaces::Prediction->SetLocalViewAngles(helper::localplayer::LastAngle);
			}
			break;
		}
		case FRAME_NET_UPDATE_END:
		{
			EntityList::Store();

			CTFPlayer* pLocal = EntityList::GetLocal();
			if (pLocal == nullptr)
				break;

			//Thirdperson::FrameStageNotify(pLocal);
			break;
		}

		default: break;
	}

	AS_FrameStage_Callback(stage);
	originalFrameStageNotify(thisptr, stage);
}

inline void HookFrameStageNotify()
{
	INSTALL_VTABLE_HOOK(FrameStageNotify, interfaces::ClientDLL, 35);

	#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IBaseClientDll::FrameStageNotify hooked\n", color);
	#endif
}