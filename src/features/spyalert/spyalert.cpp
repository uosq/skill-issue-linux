#include "spyalert.h"

#include "../../imgui/imgui.h"

#include "../../sdk/classes/player.h"
#include "../../sdk/imgui_utils/imgui_utils.h"

#include "../entitylist/entitylist.h"
#include "../triggerbot/autobackstab/autobackstab.h"

#define ALERT_TEXT "!! SPY BEHIND YOU !!"

enum class SpyStatus : uint8_t
{
	NONE = 0,
	CLOSE,
	HOLY_SHIT_ITS_CLOSE // very close
};

static uint8_t s_iSpyStatus = 0;

void SpyAlert::OnFrameStageNotify()
{
	if (!Config.misc.packed.spyalert)
		return;

	s_iSpyStatus = (int)SpyStatus::NONE;

	CTFPlayer* pLocal = EntityList::GetLocal();
	if (pLocal == nullptr)
		return;

	Vec3 localCenter = pLocal->GetCenter();
	Vec3 localOrigin = pLocal->GetAbsOrigin();

	Vec3 localViewAngles;
	interfaces::Engine->GetViewAngles(localViewAngles);

	for (const auto& entry : EntityList::GetEnemies())
	{
		if (entry.ptr == nullptr)
			continue;

		if (entry.ptr->IsDormant())
			continue;

		if (!(entry.flags & (EntityFlags::IsPlayer | EntityFlags::IsAlive)))
			continue;

		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(entry.ptr);
		if (pPlayer->m_iClass() != ETFClass::TF_CLASS_SPY)
			continue;

		Vec3 origin = entry.ptr->GetAbsOrigin();
		float distance = localOrigin.DistTo(origin);
		if (distance > 256.0f)
			continue;

		Vec3 targetCenter = entry.ptr->GetCenter();
		Vec3 viewAngles = pPlayer->m_angEyeAngles();

		if (!AutoBackstab::IsBehindAndFacingEntity(targetCenter, localCenter, viewAngles, localViewAngles))
			continue;

		s_iSpyStatus = (int)SpyStatus::CLOSE;

		if (distance <= 128.0f)
			s_iSpyStatus = (int)SpyStatus::HOLY_SHIT_ITS_CLOSE;

		return;
	}
}

void SpyAlert::OnImGui(ImDrawList* pDraw)
{
	if (!Config.misc.packed.spyalert)
		return;

	if (s_iSpyStatus == 0)
		return;

	if (interfaces::Engine->IsTakingScreenshot())
		return;

	int screenWidth = 0, screenHeight = 0;
	interfaces::Engine->GetScreenSize(screenWidth, screenHeight);

	ImVec2 size = ImGui::CalcTextSize(ALERT_TEXT);
	ImVec2 pos(screenWidth/2.0f - size.x/2.0f, screenHeight/2.0f - size.y/2.0f);
	ImU32 color
	(
		(s_iSpyStatus == (int)SpyStatus::HOLY_SHIT_ITS_CLOSE)
		? IM_COL32(255, 100, 100, 255)
		: IM_COL32(255, 255, 255, 255)
	);

	ImGui::DrawTextShadow(pDraw, pos, color, ALERT_TEXT);
}

bool SpyAlert::IsSpyNear()
{
	return s_iSpyStatus > 0;
}