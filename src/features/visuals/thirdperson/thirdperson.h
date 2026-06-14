#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"

#include "../../config/config.h"
#include "../../feature.h"

BEGIN_CONFIG(thirdperson)

	ADD_CONFIG(key, "third person key", Hotkey{})
	ADD_CONFIG(offset, "third person offset", (std::array<float, 4>{}))

END_CONFIG()

class Thirdperson
{
public:
	// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	void FrameStageNotify(CTFPlayer *pLocal);
	void OverrideView(CTFPlayer *pLocal, CViewSetup *pView);

	bool IsThirdPerson(CTFPlayer *pLocal);
	bool ShouldIgnoreBind(CTFPlayer *pLocal);

	Vector GetCameraOffset();
};

DECLARE_FEATURE(Thirdperson, thirdperson)