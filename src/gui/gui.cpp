#include "gui.h"

#include "tabs/tab_aimbot.h"
#include "tabs/tab_esp.h"
#include "tabs/tab_misc.h"
#include "tabs/tab_trigger.h"
#include "tabs/tab_code.h"
#include "tabs/tab_netvars.h"
#include "tabs/tab_radar.h"
#include "tabs/tab_config.h"
#include "tabs/tab_logs.h"
#include "tabs/tab_chams.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"
#include "../features/spectators/spectators.h"
#include "../features/esp/esp.h"
#include "../features/aimbot/aimbot.h"

int GUI::tab = 0;

void DrawTabButtons(int &tab)
{
	ImGui::BeginGroup();

	if (ImGui::Button("AIMBOT", ImVec2(-1, 0)))
		tab = TAB_AIMBOT;

	if (ImGui::Button("TRIGGER", ImVec2(-1, 0)))
		tab = TAB_TRIGGER;

	if (ImGui::Button("ESP", ImVec2(-1, 0)))
		tab = TAB_ESP;

	if (ImGui::Button("MISC", ImVec2(-1, 0)))
		tab = TAB_MISC;

	if (ImGui::Button("CHAMS", ImVec2(-1, 0)))
		tab = TAB_CHAMS;

	// if (ImGui::Button("ANTIAIM", ImVec2(-1, 0)))
	// tab = TAB_ANTIAIM;

	if (ImGui::Button("RADAR", ImVec2(-1, 0)))
		tab = TAB_RADAR;

	if (ImGui::Button("CODE", ImVec2(-1, 0)))
		tab = TAB_LUA;

	if (ImGui::Button("NETVARS", ImVec2(-1, 0)))
		tab = TAB_NETVARS;

	if (ImGui::Button("LOGS", ImVec2(-1, 0)))
		tab = TAB_LOGS;

	if (ImGui::Button("CONFIGS", ImVec2(-1, 0)))
		tab = TAB_CONFIG;

	ImGui::EndGroup();
}

void GUI::RunSpectatorList()
{
	Spectators::DrawList();
}

void GUI::RunPlayerList()
{
	if (interfaces::Engine->IsTakingScreenshot())
		return;

	ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!Settings::menu_open)
		flags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Player List", nullptr, flags))
	{
		for (const auto &entry : EntityList::GetEntities())
		{
			if (!(entry.flags & EntityFlags::IsPlayer))
				continue;

			auto *entity = static_cast<CTFPlayer *>(entry.ptr);
			if (entity == nullptr)
				continue;

			std::string name = entity->GetName();

			switch (entity->m_iTeamNum())
			{
			case TEAM_BLU:
			{
				Color color = Settings::Colors.blu_team;
				ImVec4 textColor(color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f, 255);
				ImGui::TextColored(textColor, "%s", name.c_str());
				break;
			}

			case TEAM_RED:
			{
				Color color = Settings::Colors.red_team;
				ImVec4 textColor(color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f, 255);
				ImGui::TextColored(textColor, "%s", name.c_str());
				break;
			}

			default:
				break;
			}
		}
	}
	ImGui::End();
}

void GUI::RunMainWindow()
{
	Hooks_CallHooks("ImGui");

	ImDrawList* pDraw = ImGui::GetBackgroundDrawList();
	if (pDraw == nullptr)
		return;

	if (Settings::menu_open)
	{
		pDraw->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), "Skill Issue");
		pDraw->AddText(ImVec2(10, 10 + Settings::ESP.font_size), IM_COL32(255, 255, 255, 255), "Build date: " __DATE__ " " __TIME__);
	}

	Aimbot::DrawFOVIndicator();
	ESP::OnImGui();

	if (!Settings::menu_open)
		return;

	if (helper::engine::IsTakingScreenshot())
		return;

	if (ImGui::Begin("Skill Issue", nullptr, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTable("MainTable", 2, 0))
		{
			ImGui::TableSetupColumn("Buttons", ImGuiTableColumnFlags_WidthFixed, 70);
			ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			DrawTabButtons(tab);

			ImGui::TableSetColumnIndex(1);
			if (ImGui::BeginChild("##Cheese"))
			{
				switch (tab)
				{
				case TAB_AIMBOT:
					DrawAimbotTab();
					break;
				case TAB_ESP:
					DrawESPTab();
					break;
				case TAB_MISC:
					DrawMiscTab();
					break;
				case TAB_TRIGGER:
					DrawTriggerTab();
					break;
				// case TAB_ANTIAIM: DrawAntiaimTab(); break;
				case TAB_LUA:
					DrawCodeTab();
					break;
				case TAB_NETVARS:
					DrawNetvarsTab();
					break;
				case TAB_RADAR:
					DrawRadarTab();
					break;
				case TAB_CONFIG:
					DrawConfigTab();
					break;
				case TAB_LOGS:
					DrawLogsTab();
					break;
				case TAB_CHAMS:
					DrawChamsTab();
					break;
				default:
					break;
				}
			}
			ImGui::EndChild();

			ImGui::EndTable();
		}
	}

	ImGui::End();
}

void GUI::Init()
{
}
