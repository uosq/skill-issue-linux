#include "../sdk/interfaces/interfaces.h"

#include "../features/backtrack/backtrack.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/scriptmanager/scriptmanager.h"
#include "../features/esp/esp.h"
#include "../features/spyalert/spyalert.h"
#include "../features/playerlist/playerlist.h"
#include "../features/hookmanager/hookmanager.hpp"
#include "../features/nopush/nopush.h"
#include "../features/visuals/thirdperson/thirdperson.h"

using FrameStageNotifyFn = void (*)(CHLClient* rdi, int stage);

INIT_VTABLE_HOOK(FrameStageNotify, void, (CHLClient* rdi, int stage), interfaces::ClientDLL, 35)
{
	switch (stage)
	{
		case FRAME_RENDER_START:
		{
			if (config::thirdperson::key.Get().IsActive())
			{
				CTFPlayer *pLocal = features::entities.GetLocal();
				if (pLocal && pLocal->IsAlive())
					interfaces::Prediction->SetLocalViewAngles(helper::localplayer::LastAngle);
			}
			break;
		}

		default: break;
	}

	original_FrameStageNotify(rdi, stage);

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
		features::nopush.OnFrameStageNotify();
		break;
	}

	default:
		break;
	}
}