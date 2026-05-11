#include "framestagenotify.h"

#include "../sdk/interfaces/interfaces.h"

#include "../settings/settings.h"
#include "../hooks.h"

#include "../features/backtrack/backtrack.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/scriptmanager/scriptmanager.h"
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
				CTFPlayer *pLocal = features::entities.GetLocal();
				if (pLocal && pLocal->IsAlive())
					interfaces::Prediction->SetLocalViewAngles(helper::localplayer::LastAngle);
			}
			break;
		}

		default: break;
	}

	original(rdi, stage);

	features::scriptmanager.CallHooks("FrameStageNotify", stage);

	switch (stage)
	{
	case FRAME_RENDER_START:
	{
		features::esp.OnFrameStageNotify();
		break;
	}
	case FRAME_NET_UPDATE_END:
	{
		features::entities.Store();
		features::backtrack.Store();
		features::spectators.OnFrameStageNotify();
		features::spyalert.OnFrameStageNotify();
		features::playerlist.Store();
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