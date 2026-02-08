#include "thirdperson.h"

void Thirdperson::RunHotkey()
{
	if (Settings::misc.thirdperson_key == "")
		return;

	if (helper::input::IsKeyPressed(Settings::misc.thirdperson_key))
		Settings::misc.thirdperson = !Settings::misc.thirdperson;
}

// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
void Thirdperson::Run(CTFPlayer* pLocal)
{
	RunHotkey();

	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	if (Settings::misc.thirdperson)
		pLocal->m_nForceTauntCam() = 1;

	if (!Settings::misc.thirdperson && interfaces::CInput->CAM_IsThirdPerson())
		pLocal->m_nForceTauntCam() = 0;
}