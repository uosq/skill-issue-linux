#pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

struct Thirdperson
{
	void RunHotkey()
	{
		if (g_Settings.misc.thirdperson_key == "")
			return;

		if (helper::input::IsKeyPressed(g_Settings.misc.thirdperson_key))
			g_Settings.misc.thirdperson = !g_Settings.misc.thirdperson;
	}

	// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	void Run(CTFPlayer* pLocal)
	{
		RunHotkey();

		if (pLocal == nullptr || !pLocal->IsAlive())
			return;

		if (g_Settings.misc.thirdperson)
			pLocal->m_nForceTauntCam() = 1;

		if (!g_Settings.misc.thirdperson && interfaces::CInput->CAM_IsThirdPerson())
			pLocal->m_nForceTauntCam() = 0;
	}
};

inline Thirdperson g_Thirdperson;