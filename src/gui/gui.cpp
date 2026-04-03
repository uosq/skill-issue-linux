#include "gui.h"

#include <filesystem>

#include "../features/angelscript/api/globals.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"
#include "../features/binds/binds.h"
#include "../features/logs/logs.h"
#include "../features/scriptmanager/scriptmanager.h"
#include "../features/spectators/spectators.h"
#include "../imgui/imgui_stdlib.h"

#define BASE_DIR "./skill-issue/"
#define SCRIPT_DIR "./skill-issue/scripts/"
#define CONFIG_DIR "./skill-issue/configs/"

int GUI::tab		  = 0;
bool GUI::openDeletePopup = false;

int GUI::selectedIndex	  = -1;
std::vector<std::string> GUI::configs;
char GUI::newConfigName[64]  = "\0";
bool GUI::firstOpenConfigTab = true;

std::vector<std::string> s_vScriptFiles;

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

void DrawAimbotTab()
{
	gBinds.RenderHotkey("Aimbot", Settings::Aimbot.key);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Settings::Aimbot.key->IsEnabled() ? 1.0f : 0.5f);
	{
		// ImGui::InputText("Key", Settings::Aimbot.key,
		// sizeof(Settings::Aimbot.key));

		ImGui::Checkbox("Autoshoot", &Settings::Aimbot.autoshoot);
		ImGui::Checkbox("Viewmodel Aim", &Settings::Aimbot.viewmodelaim);
		ImGui::Checkbox("Draw FOV Indicator", &Settings::Aimbot.draw_fov_indicator);
		ImGui::Checkbox("Wait For Charge", &Settings::Aimbot.waitforcharge);
		ImGui::Checkbox("Draw Path", &Settings::Aimbot.path);
		ImGui::SliderFloat("Fov", &Settings::Aimbot.fov, 0.0f, 180.0f);
		ImGui::SliderFloat("Max Sim Time", &Settings::Aimbot.max_sim_time, 0.0f, 5.0f);
		ImGui::SliderFloat("Smoothness", &Settings::Aimbot.smoothness, 0.0f, 100.0f);

		{
			constexpr const char *items[]{
			    "None",
			    "Circle",
			    "Square",
			    "Triangle",
			};
			ImGui::Combo("Indicator Style", &Settings::Aimbot.indicator, items, 4);
		}

		{
			constexpr const char *items[]{"None", "Legit", "Rage"};
			ImGui::Combo("Melee Aimbot", &Settings::Aimbot.melee, items, 3);
		}

		{
			constexpr const char *items[]{
			    "Plain",
			    "Smooth",
			    "Assistance",
			    "Silent",
			};
			ImGui::Combo("Method", &Settings::Aimbot.mode, items, 4);
		}

		{
			constexpr const char *items[]{
			    "Only Enemies",
			    "Only Teammates",
			    "Both",
			};

			ImGui::Combo("Team Selection", &Settings::Aimbot.teamMode, items, 3);
		}

		ImGui::Separator();

		ImGui::TextUnformatted("Ignore Options");
		ImGui::Checkbox("Cloaked", &Settings::Aimbot.ignorecloaked);
		ImGui::SameLine();
		ImGui::Checkbox("Ubercharged", &Settings::Aimbot.ignoreubered);
		ImGui::SameLine();
		ImGui::Checkbox("Hoovy", &Settings::Aimbot.ignorehoovy);
		ImGui::SameLine();
		ImGui::Checkbox("Bonked", &Settings::Aimbot.ignorebonked);

		ImGui::Separator();

		ImGui::Checkbox("Hold Minigun Spin", &Settings::Aimbot.hold_minigun_spin);
	}
	ImGui::PopStyleVar();
}

void DrawESPTab()
{
	if (ImGui::BeginTable("##ESPContents", 2))
	{
		ImGui::TableSetupColumn("LeftSide");
		ImGui::TableSetupColumn("RightSide");

		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Checkbox("ESP Enabled", &Settings::ESP.enabled);
		ImGui::BeginDisabled(!Settings::ESP.enabled);
		{
			ImGui::Checkbox("Name", &Settings::ESP.name);
			ImGui::Checkbox("Box", &Settings::ESP.box);
			ImGui::Checkbox("Ignore Cloaked", &Settings::ESP.ignorecloaked);
			ImGui::Checkbox("Buildings", &Settings::ESP.buildings);
			ImGui::Checkbox("Weapon", &Settings::ESP.weapon);

			{
				constexpr const char *items[]{"None", "Text", "Bar", "Both"};
				ImGui::Combo("Health", &Settings::ESP.health, items, 4);
			}

			{
				constexpr const char *items[]{"Only Enemies", "Only Teammates", "Both"};
				ImGui::Combo("Team Selection", &Settings::ESP.team_selection, items, 3);
			}
		}
		ImGui::EndDisabled();
		{
			constexpr const char *items[]{"TF2", "Arial"};
			ImGui::Combo("Font", &Settings::ESP.font, items, 2);
		}

		ImGui::TableNextColumn();

		ImGui::TextUnformatted("Conditions");

		ImGui::CheckboxFlags("Zoom", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Zoomed));
		ImGui::CheckboxFlags("Ubercharge", &Settings::ESP.fconditions,
				     static_cast<int>(ESPConditionFlags::Ubered));
		ImGui::CheckboxFlags("Jarate", &Settings::ESP.fconditions,
				     static_cast<int>(ESPConditionFlags::Jarated));
		ImGui::CheckboxFlags("Bonk", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Bonked));

		ImGui::EndTable();
	} // table end

	ImGui::Separator();

	ImGui::Checkbox("Chams", &Settings::ESP.chams);
	ImGui::SliderFloat("Chams Alpha", &Settings::ESP.chams_alpha, 0.0f, 1.0f);

	ImGui::Separator();

	ImGui::TextUnformatted("Glow");
	ImGui::SliderInt("Stencil", &Settings::ESP.stencil, 0, 10);
	ImGui::SliderInt("Blur", &Settings::ESP.blur, 0, 10);

	ImGui::Separator();

	float red[3] = {Settings::Colors.red_team.r() / 255.0f, Settings::Colors.red_team.g() / 255.0f, Settings::Colors.red_team.b() / 255.0f};
	float blu[3] = {Settings::Colors.blu_team.r() / 255.0f, Settings::Colors.blu_team.g() / 255.0f, Settings::Colors.blu_team.b() / 255.0f};
	float target[3] = {Settings::Colors.aimbot_target.r() / 255.0f, Settings::Colors.aimbot_target.g() / 255.0f, Settings::Colors.aimbot_target.b() / 255.0f};
	float weapon[3] = {Settings::Colors.weapon.r() / 255.0f, Settings::Colors.weapon.g() / 255.0f, Settings::Colors.weapon.b() / 255.0f};

	ImGui::TextUnformatted("Colors");

	if (ImGui::ColorEdit3("RED Team", red))
		Settings::Colors.red_team.SetColor(red[0] * 255.0f, red[1] * 255.0f, red[2] * 255.0f, 255.0f);

	if (ImGui::ColorEdit3("BLU Team", blu))
		Settings::Colors.blu_team.SetColor(blu[0] * 255.0f, blu[1] * 255.0f, blu[2] * 255.0f, 255.0f);

	if (ImGui::ColorEdit3("Aimbot Target", target))
		Settings::Colors.aimbot_target.SetColor(target[0] * 255.0f, target[1] * 255.0f, target[2] * 255.0f, 255.0f);

	if (ImGui::ColorEdit3("Weapon", weapon))
		Settings::Colors.weapon.SetColor(weapon[0] * 255.0f, weapon[1] * 255.0f, weapon[2] * 255.0f, 255.0f);
}

void DrawMiscTab()
{
	ImGui::BeginGroup();

	gBinds.RenderHotkey("Third Person", Settings::Misc.thirdperson_key);
	ImGui::SliderFloat4("Third Person Offset", Settings::Misc.thirdperson_offset, -100.0f, 100.0f);

	ImGui::Separator();

	ImGui::Checkbox("Spectator List", &Settings::Misc.spectatorlist);
	ImGui::Checkbox("Player List", &Settings::Misc.playerlist);
	ImGui::Checkbox("sv_pure bypass", &Settings::Misc.sv_pure_bypass);
	ImGui::Checkbox("Streamer Mode", &Settings::Misc.streamer_mode);
	ImGui::Checkbox("Bhop", &Settings::Misc.bhop);
	ImGui::Checkbox("Autostrafe", &Settings::Misc.autostrafe);
	ImGui::Checkbox("Backpack Expander", &Settings::Misc.backpack_expander);
	ImGui::Checkbox("Accept Item Drops", &Settings::Misc.accept_item_drop);

	ImGui::Checkbox("No Recoil", &Settings::Misc.norecoil);
	ImGui::Checkbox("No Engine Sleep", &Settings::Misc.no_engine_sleep);

	ImGui::Checkbox("No Scope Overlay", &Settings::Misc.no_scope_overlay);
	ImGui::Checkbox("No Zoom", &Settings::Misc.no_zoom);

	// ImGui::Checkbox("No Survey", &Settings::Misc.no_survey);

	ImGui::Separator();

	ImGui::Checkbox("Custom Fov Enabled", &Settings::Misc.customfov_enabled);

	ImGui::BeginDisabled(!Settings::Misc.customfov_enabled);
	{
		ImGui::SliderFloat("Custom Fov", &Settings::Misc.customfov, 54.0f, 120.0f);
	}
	ImGui::EndDisabled();

	ImGui::Separator();

	ImGui::Checkbox("No Viewmodel Bob", &Settings::Misc.no_viewmodel_bob);
	ImGui::SliderFloat3("Viewmodel Offset", Settings::Misc.viewmodel_offset, -20, 20.0f);
	ImGui::SliderFloat("Viewmodel Interp", &Settings::Misc.viewmodel_interp, 0.0f, 50.0f);

	ImGui::Separator();

	{
		constexpr const char *items[]{"None", "Last Record Only", "All Records"};
		ImGui::Combo("Backtrack", &Settings::Misc.backtrack, items, 3);
	}

	ImGui::Separator();

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Settings::AntiAim.warp_key->IsEnabled() ? 1.0f : 0.5f);
	{
		gBinds.RenderHotkey("Warp Key", Settings::AntiAim.warp_key);
		gBinds.RenderHotkey("Warp Recharge Key", Settings::AntiAim.warp_recharge_key);
		ImGui::SliderInt("Warp Speed", &Settings::AntiAim.warp_speed, 1, 24);
	}
	ImGui::PopStyleVar();

	ImGui::EndGroup();
}

void DrawTriggerTab()
{
	gBinds.RenderHotkey("TriggerBot", Settings::Trigger.key);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Settings::Trigger.key->IsEnabled() ? 1.0f : 0.5f);
	{
		ImGui::Checkbox("Hitscan", &Settings::Trigger.hitscan);

		{
			constexpr const char *items[]{"None", "Legit", "Rage"};
			ImGui::Combo("Auto Backstab", &Settings::Trigger.autobackstab, items, 3);
			ImGui::Combo("Auto Airblast", &Settings::Trigger.autoairblast, items, 3);
		}
	}
	ImGui::PopStyleVar();
}

void DrawAntiaimTab()
{
	ImGui::Checkbox("Enabled", &Settings::AntiAim.enabled);

	ImGui::BeginDisabled(!Settings::AntiAim.enabled);
	{
		{
			constexpr const char *items[]{"None", "Up", "Down", "Fake Up", "Fake Down", "Random"};
			ImGui::Combo("Pitch Mode", &Settings::AntiAim.pitch_mode, items, 6);
		}

		{
			constexpr const char *items[]{"None",	 "Left",      "Right",	    "Back",
						      "Forward", "Spin Left", "Spin Right", "Jitter"};

			ImGui::Combo("Real Yaw", &Settings::AntiAim.real_yaw_mode, items, 8);
			ImGui::Combo("Fake Yaw", &Settings::AntiAim.fake_yaw_mode, items, 8);
		}

		ImGui::SliderInt("Spin Speed", &Settings::AntiAim.spin_speed, 0, 10);
	}
	ImGui::EndDisabled();

	ImGui::Separator();

	ImGui::Checkbox("Fake Lag Enabled", &Settings::AntiAim.fakelag_enabled);

	ImGui::BeginDisabled(!Settings::AntiAim.fakelag_enabled);
	{
		ImGui::SliderInt("Ticks", &Settings::AntiAim.fakelag_ticks, 1, 21);
	}
	ImGui::EndDisabled();
}

struct AccessOption
{
	const char *label;
	int flag;
};

void RefreshScriptList()
{
	s_vScriptFiles.clear();

	const std::filesystem::path dir = SCRIPT_DIR;

	if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
		return;

	for (const auto &entry : std::filesystem::directory_iterator(dir))
	{
		if (!entry.is_regular_file())
			continue;

		const auto &path = entry.path();

		if (path.extension() == ".as")
			s_vScriptFiles.emplace_back(path.filename().string());
	}

	std::sort(s_vScriptFiles.begin(), s_vScriptFiles.end());
}

std::string ToLower(std::string str)
{
	// wtf is this syntax
	// why the fuck do we need to specify the begin, end and then the
	// fucking begin again?? fuck you C++
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
	return str;
}

bool ContainsInsensitive(const std::string &text, const std::string &search)
{
	// bro why can't it just be a fucking bool and return on a parameter???
	// fuck you c++
	return ToLower(text).find(ToLower(search)) != std::string::npos;
}

void DrawLuaTab()
{
	static std::string s_SearchString = "";

	ImGui::BeginGroup();

	if (ImGui::BeginTabBar("LuaTab"))
	{
		if (ImGui::BeginTabItem("Scripts"))
		{
			if (ImGui::Button("Refresh"))
				RefreshScriptList();

			ImGui::SameLine();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::InputText("##Search Files", &s_SearchString);

			bool bIsSearchEmpty = s_SearchString.empty();

			if (!s_vScriptFiles.empty())
			{
				if (ImGui::BeginChild("##ScriptList"))
				{
					int columns = std::max(1, (int)(ImGui::GetContentRegionAvail().x / 250.0f));

					if (ImGui::BeginTable("##ScriptTable", columns))
					{
						for (int i = 0; i < s_vScriptFiles.size(); i++)
						{
							const std::string &file = s_vScriptFiles[i];

							if (!bIsSearchEmpty &&
							    !ContainsInsensitive(file, s_SearchString))
								continue;

							ImGui::TableNextColumn();

							std::string fullPath = std::string(SCRIPT_DIR) + "/" + file;

							Script &script	     = ScriptManager::GetOrCreate(fullPath);

							bool wasLoaded	     = script.loaded;

							if (ImGui::Checkbox(file.c_str(), &wasLoaded))
							{
								if (wasLoaded)
									ScriptManager::Load(script);
								else
									ScriptManager::Unload(script);
							}
						}

						ImGui::EndTable();
					}
					ImGui::EndChild();
				}
			}
			else
			{
				std::string dir =
				    std::string(interfaces::Engine->GetGameDirectory()) + "/skill-issue/scripts/";
				ImGui::Text("Add scripts in %s", dir.c_str());
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Restrictions"))
		{
			static const AccessOption options[]{
			    {"Allow Engine Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_ENGINE},
			    {"Allow Client Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_CLIENT},
			    {"Allow ClientState Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_CLIENTSTATE},
			    {"Allow Common Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_COMMON},
			    {"Allow Draw Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_DRAW},
			    {"Allow EntityList Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_ENTITYLIST},
			    {"Allow Hooks Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_HOOKS},
			    {"Allow ImGui Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_IMGUI},
			    {"Allow Input Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_INPUT},
			    {"Allow Materials Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_MATERIALS},
			    {"Allow Render Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_RENDER},
			    {"Allow FileSystem Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_FILESYSTEM},
			    {"Allow Math Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_MATH},
			    {"Allow Array Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_ARRAY},
			    {"Allow Dictionary Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_DICT},
			    {"Allow DateTime Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_DATETIME},
			    {"Allow String Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_STRING},
			};

			int &mask   = GetScriptAccessMask();

			int columns = std::max(1, (int)(ImGui::GetContentRegionAvail().x / 250.0f));

			if (ImGui::BeginTable("##fdgidfojg", columns))
			{
				for (int i = 0; i < IM_ARRAYSIZE(options); i++)
				{
					ImGui::TableNextColumn();
					ImGui::CheckboxFlags(options[i].label, &mask, options[i].flag);
				}

				ImGui::EndTable();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::EndGroup();
}

std::string &GetNetvarSearchString()
{
	static std::string s_Search;
	return s_Search;
}

void DrawParsedNetvarData(const std::vector<NetvarClassEntry> &classes)
{
	const std::string &strSearch = GetNetvarSearchString();
	bool bIsSearchEmpty	     = strSearch.empty();

	for (const auto &cls : classes)
	{
		if (!bIsSearchEmpty && !ContainsInsensitive(cls.className, strSearch))
			continue;

		if (ImGui::TreeNode(cls.className.c_str()))
		{
			if (cls.members.empty())
				ImGui::TextDisabled("No members");
			else
				for (const auto &name : cls.members)
					ImGui::BulletText("%s", name.c_str());

			ImGui::TreePop();
		}
	}
}

void DrawNetVarsTab()
{
	float maxSize = ImGui::GetContentRegionAvail().x;
	ImGui::SetNextItemWidth(maxSize);
	ImGui::InputText("##SearchBar", &GetNetvarSearchString());
	if (ImGui::BeginChild("NetvarContent"))
		DrawParsedNetvarData(Netvars::m_netvarUIVector);
	ImGui::EndChild();
}

void DrawRadarTab()
{
	ImGui::Checkbox("Enabled", &Settings::Radar.enabled);
	ImGui::SliderInt("Size", &Settings::Radar.size, 1, 300);
	ImGui::SliderInt("Icon Size", &Settings::Radar.icon_size, 1, 15);
	ImGui::SliderInt("Range", &Settings::Radar.range, 10, 3000);

	ImGui::Separator();
	ImGui::Checkbox("Players", &Settings::Radar.players);
	ImGui::Checkbox("Projectiles", &Settings::Radar.projectiles);
	// ImGui::Checkbox("Objective", &Settings::Radar.objective);
	ImGui::Checkbox("Buildings", &Settings::Radar.buildings);
}

void RefreshConfigList(const std::string &folder)
{
	GUI::configs.clear();

	if (!std::filesystem::exists(folder))
		std::filesystem::create_directories(folder);

	for (const auto &entry : std::filesystem::directory_iterator(folder))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".ini")
		{
			GUI::configs.push_back(entry.path().stem().string());
		}
	}
}

void DrawConfigTab()
{
	const std::string configFolder = CONFIG_DIR;

	if (GUI::firstOpenConfigTab)
	{
		RefreshConfigList(configFolder);
		GUI::firstOpenConfigTab = false;
	}

	if (ImGui::Button("Refresh"))
	{
		RefreshConfigList(configFolder);
		GUI::selectedIndex = -1;
	}

	ImGui::SameLine();

	float buttonWidth = 120.0f;
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - buttonWidth - ImGui::GetStyle().ItemSpacing.x);

	ImGui::InputText("##NewConfigName", GUI::newConfigName, sizeof(GUI::newConfigName));

	ImGui::SameLine();

	if (ImGui::Button("Create & Save", ImVec2(buttonWidth, 0)))
	{
		if (strlen(GUI::newConfigName) > 0)
		{
			std::string filename = std::string(GUI::newConfigName) + ".ini";
			std::string fullPath = configFolder + "/" + filename;

			Settings::Save(fullPath);
			RefreshConfigList(configFolder);
			GUI::newConfigName[0] = '\0';
		}
	}

	ImGui::Separator();

	if (ImGui::BeginChild("##Configs"))
	{
		// config list
		for (int i = 0; i < GUI::configs.size(); ++i)
		{
			bool isSelected = GUI::selectedIndex == i;

			if (ImGui::Selectable(GUI::configs[i].c_str(), isSelected))
				GUI::selectedIndex = i;

			if (isSelected)
			{
				std::string fullPath = configFolder + "/" + GUI::configs[i] + ".ini";

				if (ImGui::Button("Load"))
					Settings::Load(fullPath);

				ImGui::SameLine();

				if (ImGui::Button("Save"))
					Settings::Save(fullPath);

				ImGui::SameLine();

				if (ImGui::Button("Delete"))
					GUI::openDeletePopup = true;

				// this doesn't look good to me, should I
				// change it?
				if (GUI::openDeletePopup)
					ImGui::OpenPopup("ConfirmDelete");

				if (ImGui::BeginPopupModal("ConfirmDelete", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Delete selected config?");
					ImGui::Separator();

					if (ImGui::Button("Yes", ImVec2(120, 0)))
					{
						std::filesystem::remove(fullPath);
						RefreshConfigList(configFolder);
						GUI::selectedIndex   = -1;
						GUI::openDeletePopup = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("No", ImVec2(120, 0)))
					{
						GUI::openDeletePopup = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
		}
	}

	ImGui::EndChild();
}

void DrawLogsTab()
{
	ImGui::BeginChild("##LogsContent");

	for (const auto &entry : Logs::GetLogs())
	{
		ImVec4 color;
		switch (entry.level)
		{
		case LogLevel::INFO:
			color = ImVec4(1, 1, 1, 1);
			ImGui::TextColored(color, "%s - %s", "Info", entry.text.c_str());
			break;
		case LogLevel::WARN:
			color = ImVec4(1, 1, 0, 1);
			ImGui::TextColored(color, "%s - %s", "Warn", entry.text.c_str());
			break;
		case LogLevel::ERROR:
			color = ImVec4(1, 0, 0, 1);
			ImGui::TextColored(color, "%s - %s", "Error", entry.text.c_str());
			break;
		}
	}

	ImGui::EndChild();
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
					DrawLuaTab();
					break;
				case TAB_NETVARS:
					DrawNetVarsTab();
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
	configs.reserve(5);
	selectedIndex	   = -1;
	firstOpenConfigTab = true;
	openDeletePopup = false;
}
