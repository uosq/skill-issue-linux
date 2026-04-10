#include "tab_config.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_stdlib.h"

#include "../../features/configmanager/configmgr.h"

#define CONFIG_DIR "./skill-issue/configs/"

static int s_iSelectedIndex = -1;
static std::string s_strNewConfigName = "";
static bool s_bOpenDeletePopup = false;
static bool s_bFirstTimeOpen = true;

void DrawConfigTab()
{
	const std::string configFolder = CONFIG_DIR;
	auto& configs = ConfigManager::GetConfigs();

	if (s_bFirstTimeOpen)
	{
		ConfigManager::RefreshConfigs();
		s_bFirstTimeOpen = false;
	}

	if (ImGui::Button("Refresh"))
	{
		ConfigManager::RefreshConfigs();
		s_iSelectedIndex = -1;
	}

	ImGui::SameLine();

	float buttonWidth = 120.0f;
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - buttonWidth - ImGui::GetStyle().ItemSpacing.x);

	ImGui::InputText("##NewConfigName", &s_strNewConfigName, sizeof(s_strNewConfigName));

	ImGui::SameLine();

	if (ImGui::Button("Create & Save", ImVec2(buttonWidth, 0)))
	{
		if (s_strNewConfigName.length() > 0)
		{
			std::string filename = s_strNewConfigName + ".ini";
			std::string fullPath = configFolder + filename;

			ConfigManager::Save(fullPath);
			ConfigManager::RefreshConfigs();
		}
	}

	ImGui::Separator();

	if (ImGui::BeginChild("##Configs"))
	{
		// config list
		for (int i = 0; i < configs.size(); ++i)
		{
			bool isSelected = s_iSelectedIndex == i;

			if (ImGui::Selectable(configs[i].c_str(), isSelected))
				s_iSelectedIndex = i;

			if (isSelected)
			{
				std::string fullPath = configFolder + configs[i] + ".ini";

				if (ImGui::Button("Load"))
				{
					(void)ConfigManager::Load(fullPath);
					ConfigManager::RefreshConfigs();
				}

				ImGui::SameLine();

				if (ImGui::Button("Save"))
				{
					(void)ConfigManager::Save(fullPath);
					ConfigManager::RefreshConfigs();
				}

				ImGui::SameLine();

				if (ImGui::Button("Delete"))
					s_bOpenDeletePopup = true;

				// this doesn't look good to me
				// should I change it?
				if (s_bOpenDeletePopup)
					ImGui::OpenPopup("ConfirmDelete");

				if (ImGui::BeginPopupModal("ConfirmDelete", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Delete selected config?");
					ImGui::Separator();

					if (ImGui::Button("Yes", ImVec2(120, 0)))
					{
						ConfigManager::Delete(fullPath);
						ConfigManager::RefreshConfigs();
						s_iSelectedIndex   = -1;
						s_bOpenDeletePopup = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("No", ImVec2(120, 0)))
					{
						s_bOpenDeletePopup = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
		}
	}

	ImGui::EndChild();
}