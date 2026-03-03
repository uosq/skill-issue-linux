#include "thirdperson.h"

void Thirdperson::RunHotkey()
{
	if (strcmp(Settings::Misc.thirdperson_key, "") == 0)
		return;

	if (helper::input::IsKeyPressed(Settings::Misc.thirdperson_key))
		Settings::Misc.thirdperson = !Settings::Misc.thirdperson;
}

// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
void Thirdperson::Run(CTFPlayer* pLocal)
{
	RunHotkey();

	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	if (Settings::Misc.thirdperson)
		pLocal->m_nForceTauntCam() = 1;

	if (!Settings::Misc.thirdperson && interfaces::CInput->CAM_IsThirdPerson())
		pLocal->m_nForceTauntCam() = 0;
}