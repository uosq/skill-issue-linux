#include "framestagenotify.h"

#include "../settings/settings.h"

#include "../sdk/interfaces/interfaces.h"

#if 0
#include "../features/backtrack/backtrack.h"
#endif

#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"
#include "../features/esp/esp.h"

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
	
		#if 0
		// temporary
		for (const auto& enemy : EntityList::GetEnemies())
		{
			if (enemy.ptr == nullptr)
				continue;

			Backtrack::Store(pLocal, enemy);
		}
		#endif

		Spectators::RunMain(pLocal);
		//Thirdperson::FrameStageNotify(pLocal);
		break;
	}

	default:
		break;
	}

	Hooks_CallHooks("FrameStageNotify", [&](asIScriptContext *ctx) { ctx->SetArgDWord(0, stage); });
	originalFrameStageNotify(thisptr, stage);

	ESP::OnFrameStageNotify(stage);
}

void HookFrameStageNotify()
{
	INSTALL_VTABLE_HOOK(FrameStageNotify, interfaces::ClientDLL, 35);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IBaseClientDll::FrameStageNotify hooked\n", color);
#endif
}