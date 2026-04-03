#include "warp.h"
#include "../../imgui/imgui.h"
#include "../../settings/settings.h"
#include <cstdio>

namespace Warp
{
	int m_iStoredTicks	  = 0;
	WarpState m_iDesiredState = WarpState::WAITING;
	bool m_bShifting	  = false;
	bool m_bRecharging	  = false;
	int m_iShiftAmount	  = 0;

	void Reset()
	{
		m_iDesiredState = WarpState::WAITING;
		m_iStoredTicks	= 0;
		m_bShifting	= false;
		m_bRecharging	= false;
		m_iShiftAmount	= 0;
	}

	int GetMaxTicks()
	{
		static ConVar *sv_maxusrcmdprocessticks = interfaces::Cvar->FindVar("sv_maxusrcmdprocessticks");
		return sv_maxusrcmdprocessticks->GetInt();
	}

	void RunCreateMove(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
	{
		if (!Settings::AntiAim.warp_key->IsActive() && !Settings::AntiAim.warp_recharge_key->IsActive())
			Warp::m_iDesiredState = WarpState::WAITING;

		if (!Settings::AntiAim.warp_key->IsEnabled())
			return;

		if (!interfaces::Engine->IsInGame() || !interfaces::Engine->IsConnected())
			return;

		if (interfaces::EngineVGui->IsGameUIVisible() || interfaces::EngineVGui->IsConsoleVisible())
			return;

		if (Settings::AntiAim.warp_key->IsActive() && Warp::m_iStoredTicks > 0)
		{
			Warp::m_iDesiredState = WarpState::RUNNING;
			return;
		}

		if (Settings::AntiAim.warp_recharge_key->IsActive() && Warp::m_iStoredTicks < GetMaxTicks())
		{
			Warp::m_iDesiredState = WarpState::RECHARGING;
			return;
		}
	}

	void DrawContents()
	{
		ImVec2 pos	     = ImGui::GetCursorScreenPos();
		ImDrawList *draw     = ImGui::GetWindowDrawList();
		constexpr int width  = 100;
		constexpr int height = 20;

		float ratio	     = static_cast<float>(m_iStoredTicks) / GetMaxTicks();

		ImVec2 min	     = {pos.x, pos.y};
		ImVec2 max	     = {pos.x + (width * ratio), pos.y + height};

		if (m_iStoredTicks > 0)
			draw->AddRectFilled(min, max, IM_COL32(0, 119, 200, 255), 2.0f);

		char chrText[12];
		sprintf(chrText, "%i/%i", Warp::m_iStoredTicks, GetMaxTicks());
		ImVec2 textSize = ImGui::CalcTextSize(chrText);

		draw->AddText(
		    ImVec2(pos.x + width / 2.0f - textSize.x / 2.0f, pos.y + height / 2.0f - textSize.y / 2.0f),
		    IM_COL32(255, 255, 255, 255), chrText);
	}

	void RunWindow()
	{
		int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
		if (!Settings::menu_open)
			flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;

		if (ImGui::Begin("Stored Ticks", nullptr, flags))
		{
			DrawContents();
			ImGui::Dummy(ImVec2(100, 20));
		}

		ImGui::End();
	}

	bool IsValidWeapon(CTFWeaponBase *pWeapon)
	{
		return pWeapon->GetWeaponType() == EWeaponType::HITSCAN;
	}
} // namespace Warp