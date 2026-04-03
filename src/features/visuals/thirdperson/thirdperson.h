#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../settings/settings.h"

namespace Thirdperson
{
	// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	void FrameStageNotify(CTFPlayer *pLocal);
	void OverrideView(CTFPlayer *pLocal, CViewSetup *pView);

	bool IsThirdPerson(CTFPlayer *pLocal);
	bool ShouldIgnoreBind(CTFPlayer *pLocal);

	Vector GetCameraOffset();
}; // namespace Thirdperson