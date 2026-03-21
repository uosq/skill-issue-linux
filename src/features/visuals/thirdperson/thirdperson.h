#pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings/settings.h"

namespace Thirdperson
{
	// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	void FrameStageNotify(CTFPlayer* pLocal);
	void OverrideView(CTFPlayer* pLocal, CViewSetup* pView);

	bool IsThirdPerson(CTFPlayer* pLocal);
	bool ShouldIgnoreBind(CTFPlayer* pLocal);

	Vector GetCameraOffset();
};