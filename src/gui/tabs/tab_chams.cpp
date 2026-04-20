#include "tab_chams.h"

#include "../../imgui/texteditor/TextEditor.h"

#include "../../settings/settings.h"

#include "../../features/chams/chams.h"

#define MATERIALS_DIR "./skill-issue/materials/"

#if 0
static std::string GenerateMaterialName()
{
	return "material_" + std::to_string(interfaces::GlobalVars->realtime);
}
#endif

void DrawChamsTab()
{
	static TextEditor editor{};
	static int selected = -1;
	static int last_selected = -1;
	
	auto& materials = Chams::GetMaterials();

	constexpr int TABLE_FLAGS =ImGuiTableFlags_Resizable | 
				ImGuiTableFlags_BordersInnerV |
				ImGuiTableFlags_SizingStretchSame;

	if (ImGui::BeginTable("##ChamsTable", 2, TABLE_FLAGS))
	{
		ImGui::TableSetupColumn("Materials", ImGuiTableColumnFlags_WidthStretch, 0.35f);
		ImGui::TableSetupColumn("Editor", ImGuiTableColumnFlags_WidthStretch, 0.65f);

		ImGui::TableNextRow();
	
		// Material List
		ImGui::TableNextColumn();
		{
			if (ImGui::BeginChild("##MatList"))
			{
				ImGui::Checkbox("Enabled##Chams", &Settings::ESP.chams);
				//ImGui::SliderFloat("Alpha##Chams", &Settings::ESP.chams_alpha, 0.0f, 1.0f);

				ImGui::Separator();

				for (int i = 0; i < materials.size(); i++)
				{
					auto& mat = materials[i];
					if (!mat->IsValidMat())
						continue;

					bool is_selected = (selected == i);

					if (ImGui::Selectable(mat->GetDisplayName().c_str(), is_selected))
						selected = i;
				}

				ImGui::Separator();

				{
					if (ImGui::BeginPopup("NewMaterialPopup"))
					{
						static char text[50] = "name here";
						ImGui::InputText("Material Name##Chams", text, sizeof(text));

						if (ImGui::Button("Create"))
						{
							std::string vmt =
							"UnlitGeneric\n"
							"{\n"
							"\t$basetexture \"white\"\n"
							"}";

							materials.emplace_back(std::make_shared<ChamsMaterial>(text, vmt));

							selected = materials.size() - 1;
							editor.SetText(vmt);

							ImGui::CloseCurrentPopup();
						}

						ImGui::SameLine();

						if (ImGui::Button("Cancel"))
							ImGui::CloseCurrentPopup();

						ImGui::EndPopup();
					}

					if (ImGui::Button("+ New"))
						ImGui::OpenPopup("NewMaterialPopup");
				}
				ImGui::SameLine();
				if (ImGui::Button("- Delete") && selected != -1)
				{
					auto& mat = materials[selected];
					if (mat->IsValidMat())
					{
						// delete material
						Chams::RemoveMaterial(mat->GetInternalName());
						selected = -1;
					}
				}
			}
			ImGui::EndChild();
		}

		// Editor
		ImGui::TableNextColumn();
		{
			if (ImGui::BeginChild("##MatEditor"))
			{
				if (selected != -1)
				{
					auto& mat = materials[selected];

					// load text when selection changes
					if (selected != last_selected)
					{
						editor.SetText(mat->GetVMT());
						last_selected = selected;
					}

					ImGui::Text("Editing: %s", mat->GetDisplayName().c_str());
					ImGui::Separator();

					ImVec2 avail = ImGui::GetContentRegionAvail();
					editor.Render("##Editor", ImVec2(avail.x, avail.y - 25));

					if (ImGui::Button("Apply & Reload"))
					{
						const std::string& text = editor.GetText();

						mat->SetVMT(text);
						mat->Refresh();
					}

					ImGui::SameLine();

					if (ImGui::Button("Get VMT"))
						editor.SetText(mat->GetVMT());

					ImGui::SameLine();

					bool used = mat->IsUsed();
					if (ImGui::Checkbox("Used", &used))
						mat->SetUsed(used);

					float alpha = mat->GetAlpha();
					ImGui::SliderFloat("Alpha##Chams", &alpha, 0.0f, 1.0f);
					mat->SetAlpha(alpha);
				}
				else
					ImGui::TextUnformatted("Select a material to edit");
			}
			ImGui::EndChild();
		}
	
		ImGui::EndTable();
	}
}