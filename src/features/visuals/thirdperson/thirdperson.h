#pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings/settings.h"

namespace Thirdperson
{
	void RunHotkey();
	// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	void Run(CTFPlayer* pLocal);
};