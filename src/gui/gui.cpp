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
#include "tabs/tab_mateditor.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/spectators/spectators.h"
#include "../features/esp/esp.h"
#include "../features/aimbot/aimbot.h"
#include "../features/infopanel/infopanel.h"
#include "../features/spyalert/spyalert.h"
#include "../features/playerlist/playerlist.h"
#include "../features/radar/radar.h"
#include "../features/warp/warp.h"
#include "../features/logs/logs.h"

extern ImFont* IMFONT_TF2Build;
extern ImFont* IMFONT_Arial;
extern const unsigned int Arial_compressed_size;
extern const unsigned char Arial_compressed_data[668199];

void GUI::RunSpectatorList()
{
	features::spectators.DrawList();
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
		for (const auto &entry : features::entities.GetEntities())
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
				Color color = Config.colors.blu_team;
				ImVec4 textColor(color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f, 255);
				ImGui::TextColored(textColor, "%s", name.c_str());
				break;
			}

			case TEAM_RED:
			{
				Color color = Config.colors.red_team;
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

static void CheckToggleMenu()
{
	if (ImGui::IsKeyPressed(ImGuiKey_Insert, false) || ImGui::IsKeyPressed(ImGuiKey_F11, false))
	{
		Settings::menu_open = !Settings::menu_open;
		interfaces::Surface->SetCursorAlwaysVisible(Settings::menu_open);
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
	{
		Settings::menu_open = false;
		interfaces::Surface->SetCursorAlwaysVisible(Settings::menu_open);
	}
}

static bool BeginFont()
{
	bool pushfont = false;
	switch (static_cast<ESPFont>(Config.esp.font.selected))
	{
		case ESPFont::TF2BUILD:
		ImGui::PushFont(IMFONT_TF2Build, Config.esp.font.size);
		pushfont = true;
		break;
		case ESPFont::ARIAL:
		ImGui::PushFont(IMFONT_Arial, Config.esp.font.size);
		pushfont = true;
		break;
		case ESPFont::INVALID:
		case ESPFont::COUNT:
		{
			features::logs.Error("Invalid font!");
			break;
		}
	}

	return pushfont;
}

static void EndFont(bool push)
{
	if (push) ImGui::PopFont();
}

static void BeginFrame()
{
	cursor = ImGui::GetMouseCursor();

	features::binds.Update();

	CheckToggleMenu();

	if (Config.warp.key->IsEnabled())
		features::warp.RunWindow();

	if (Config.radar.packed.enabled)
		features::radar.Run();

	if (Config.misc.packed.spectatorlist)
		GUI::RunSpectatorList();
}

static void EndFrame()
{
	features::binds.DrawWindow(Settings::menu_open);
}

void GUI::RunMainWindow()
{
	bool pushfont = BeginFont();
	BeginFrame();

	features::scriptmanager.CallHooks("ImGui");

	ImDrawList* pDraw = ImGui::GetBackgroundDrawList();
	if (pDraw == nullptr)
	{
		if (pushfont)
			EndFont(pushfont);

		return;
	}

	if (Settings::menu_open)
	{
		pDraw->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), "Skill Issue");
		pDraw->AddText(ImVec2(10, 10 + Config.esp.font.size), IM_COL32(255, 255, 255, 255), "Build date: " __DATE__ " " __TIME__);
	}

	features::aimbot.OnImGui(pDraw);
	features::esp.OnImGui();
	features::infopanel.OnImGui(Settings::menu_open);
	features::spyalert.OnImGui(pDraw);
	features::playerlist.DrawWindow();

	if (Settings::menu_open && !helper::engine::IsTakingScreenshot())
	{
		if (ImGui::Begin("Skill Issue", nullptr, ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("MainTabBar"))
			{
				if (ImGui::BeginTabItem("AIMBOT"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawAimbotTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("ESP"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawESPTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("MISC"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawMiscTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("TRIGGER"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawTriggerTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("LUA"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawCodeTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("NETVARS"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawNetvarsTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("RADAR"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawRadarTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("CONFIG"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawConfigTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("LOGS"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawLogsTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("CHAMS"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawMaterialEditor();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}

	EndFrame();

	if (pushfont)
		EndFont(pushfont);
}

void GUI::Init()
{
}
