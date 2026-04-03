#pragma once

#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"

#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/visuals/customfov/customfov.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(FrameStageNotify, void, (CHLClient * thisptr, int stage))
{
	switch (stage)
	{
	case FRAME_RENDER_START:
	{
		if (Settings::Misc.thirdperson_key->IsActive())
		{
			CTFPlayer *pLocal = EntityList::GetLocal();
			if (pLocal == nullptr || !pLocal->IsAlive())
				break;

			interfaces::Prediction->SetLocalViewAngles(helper::localplayer::LastAngle);
		}
		break;
	}
	case FRAME_NET_UPDATE_END:
	{
		EntityList::Store();

		CTFPlayer *pLocal = EntityList::GetLocal();
		if (pLocal == nullptr)
			break;

		Spectators::RunMain(pLocal);
		//Thirdperson::FrameStageNotify(pLocal);
		break;
	}

	default:
		break;
	}

	Hooks_CallHooks("FrameStageNotify", [&](asIScriptContext *ctx) { ctx->SetArgDWord(0, stage); });
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