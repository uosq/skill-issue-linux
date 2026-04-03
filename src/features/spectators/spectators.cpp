#include "spectators.h"
#include "../../imgui/imgui.h"
#include "../../settings/settings.h"
#include "../entitylist/entitylist.h"

static bool s_bSpectated = false;
static std::vector<CTFPlayer *> s_vSpectatorList;

void Spectators::RunMain(CTFPlayer *pLocal)
{
	Reset();
	s_bSpectated = IsSpectated(pLocal, s_vSpectatorList);
}

void Spectators::OnLevelInitPostEntity()
{
	Reset();
	s_vSpectatorList.reserve(interfaces::Engine->GetMaxClients());
}

void Spectators::Reset()
{
	s_bSpectated = false;
	s_vSpectatorList.clear();
}

bool Spectators::IsSpectated(CTFPlayer *pTarget, std::vector<CTFPlayer *> &out)
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

bool Spectators::IsLocalPlayerSpectated()
{
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
		if (s_bSpectated && !s_vSpectatorList.empty())
		{
			for (auto pPlayer : s_vSpectatorList)
			{
				if (pPlayer == nullptr)
					continue;

				int iObserverMode  = pPlayer->m_iObserverMode();
				bool isfirstperson = iObserverMode == OBS_MODE_IN_EYE;

				ImGui::TextColored(isfirstperson ? ImVec4(1.0, 0.5, 0.5, 1.0)
								 : ImVec4(1.0, 1.0, 1.0, 1.0),
						   "%s", pPlayer->GetName().c_str());
			}
		}
	}

	ImGui::End();
}

void Spectators::OnLevelShutdown()
{
	Reset();
}