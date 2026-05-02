#include "framestagenotify.h"

#include "../sdk/interfaces/interfaces.h"

#include "../settings/settings.h"
#include "../hooks.h"

#include "../features/backtrack/backtrack.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"
#include "../features/esp/esp.h"
#include "../features/spyalert/spyalert.h"
#include "../features/playerlist/playerlist.h"

using FrameStageNotifyFn = void (*)(CHLClient* rdi, int stage);

static void FrameStageNotify(CHLClient* rdi, int stage)
{
	auto original = VMTHooks::Client.GetOriginal<FrameStageNotifyFn>(35);

	switch (stage)
	{
		case FRAME_RENDER_START:
		{
			if (Config.misc.thirdperson_key->IsActive())
			{
				CTFPlayer *pLocal = EntityList::GetLocal();
				if (pLocal && pLocal->IsAlive())
					interfaces::Prediction->SetLocalViewAngles(helper::localplayer::LastAngle);
			}
			break;
		}

		default: break;
	}

	original(rdi, stage);

	Hooks_CallHooks("FrameStageNotify", [&](asIScriptContext *ctx) { ctx->SetArgDWord(0, stage); });

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
		Playerlist::Store();
		break;
	}

	default:
		break;
	}
}

void HookFrameStageNotify()
{
	VMTHooks::Client.Hook(35, &FrameStageNotify);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IBaseClientDll::FrameStageNotify hooked\n", color);
#endif
}