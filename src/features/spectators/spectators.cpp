#include "spectators.h"
#include "../../imgui/imgui.h"
#include "../../settings/settings.h"
#include "../entitylist/entitylist.h"
#include <mutex>

struct SpectatorData
{
	std::string name;
	bool isFirstPerson;
};

static bool s_bSpectated = false;
static std::mutex s_Mutex;
static std::vector<SpectatorData> s_vCachedSpectatorList;

void Spectators::OnFrameStageNotify()
{
	Reset();

	CTFPlayer* pLocal = EntityList::GetLocal();

	bool bTempSpectated = false;
	std::vector<CTFPlayer*> tempPlayerPointers;

	if (pLocal) 
		bTempSpectated = IsSpectated(pLocal, tempPlayerPointers);

	std::vector<SpectatorData> tempCachedData;
	if (bTempSpectated)
	{
		for (auto pPlayer : tempPlayerPointers)
		{
			if (pPlayer == nullptr || pPlayer->GetIndex() == -1)
				continue;

			SpectatorData data;
			data.name = pPlayer->GetName();
			data.isFirstPerson = (pPlayer->m_iObserverMode() == OBS_MODE_IN_EYE);
			tempCachedData.push_back(data);
		}
	}

	std::lock_guard<std::mutex> lock(s_Mutex);
	s_bSpectated = bTempSpectated;
	s_vCachedSpectatorList = std::move(tempCachedData);
}

void Spectators::OnLevelInitPostEntity()
{
	Reset();
	std::lock_guard<std::mutex> lock(s_Mutex);
	s_vCachedSpectatorList.reserve(interfaces::Engine->GetMaxClients());
}

void Spectators::Reset()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	s_bSpectated = false;
	s_vCachedSpectatorList.clear();
}

bool Spectators::IsSpectated(CTFPlayer *pTarget, std::vector<CTFPlayer*> &out)
{
	for (const auto &entry : EntityList::GetEntities())
	{
		if (entry.ptr == nullptr)
			continue;

		if ((entry.flags & EntityFlags::IsAlive) != 0)
			continue;

		if ((entry.flags & EntityFlags::IsPlayer) == 0)
			continue;

		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(entry.ptr);
		int iObserverMode  = pPlayer->m_iObserverMode();

		if (iObserverMode == OBS_MODE_NONE)
			continue;

		CBaseEntity *pSpectatedTarget = HandleAs<CBaseEntity *>(pPlayer->m_hObserverTarget());
		if (pSpectatedTarget == nullptr)
			continue;

		if (pSpectatedTarget != pTarget)
			continue;

		out.emplace_back(pPlayer);
	}

	return !out.empty();
}

bool Spectators::IsLocalPlayerSpectated(int& amount)
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	amount = s_vCachedSpectatorList.size();
	return s_bSpectated;
}

void Spectators::DrawList()
{
	if (helper::engine::IsTakingScreenshot())
		return;

	ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

	if (!Settings::menu_open)
		flags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Spectator List", nullptr, flags))
	{
		std::lock_guard<std::mutex> lock(s_Mutex);

		if (s_bSpectated && !s_vCachedSpectatorList.empty())
		{
			for (auto spec : s_vCachedSpectatorList)
			{
				ImVec4 color = spec.isFirstPerson ? ImVec4(1.0, 0.5, 0.5, 1.0) : ImVec4(1.0, 1.0, 1.0, 1.0);
				ImGui::TextColored(color, "%s", spec.name.c_str());
			}
		}
	}

	ImGui::End();
}

void Spectators::OnLevelShutdown()
{
	Reset();
}