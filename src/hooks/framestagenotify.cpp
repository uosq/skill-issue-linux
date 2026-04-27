#include "framestagenotify.h"

#include "../settings/settings.h"

#include "../sdk/interfaces/interfaces.h"

#include "../features/backtrack/backtrack.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"
#include "../features/esp/esp.h"
#include "../features/spyalert/spyalert.h"

DECLARE_VTABLE_HOOK(FrameStageNotify, void, (CHLClient * thisptr, int stage))
{
	switch (stage)
	{
		case FRAME_RENDER_START:
		{
			if (Settings::Misc.thirdperson_key->IsActive())
			{
				CTFPlayer *pLocal = EntityList::GetLocal();
				if (pLocal && pLocal->IsAlive())
					interfaces::Prediction->SetLocalViewAngles(helper::localplayer::LastAngle);
			}
			break;
		}

		default: break;
	}

	Hooks_CallHooks("FrameStageNotify", [&](asIScriptContext *ctx) { ctx->SetArgDWord(0, stage); });
	originalFrameStageNotify(thisptr, stage);

	switch (stage)
	{
	case FRAME_RENDER_START:
	{
		ESP::OnFrameStageNotify();
		break;
	}
	case FRAME_NET_UPDATE_END:
	{
		EntityList::Store();
		Backtrack::Store();
		Spectators::OnFrameStageNotify();
		SpyAlert::OnFrameStageNotify();
		break;
	}

	default:
		break;
	}
}

void HookFrameStageNotify()
{
	INSTALL_VTABLE_HOOK(FrameStageNotify, interfaces::ClientDLL, 35);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IBaseClientDll::FrameStageNotify hooked\n", color);
#endif
}