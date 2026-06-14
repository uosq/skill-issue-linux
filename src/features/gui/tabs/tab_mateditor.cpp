#include <filesystem>
#include <string>
#include <vector>

#include "../../../thirdparty/imgui/imgui.h"
#include "../../../thirdparty/imgui/texteditor/TextEditor.h"

#include "../../../sdk/definitions/fnv.h"

#include "../../config/config.h"
#include "../../MaterialManager/materialmanager.h"

#include "../../chams/chams.h"
#include "../../backtrack/backtrack.h"

void ToggleMaterialUsage(ConfigValue<int>& configItem, int slotIndex, const char* label)
{
        if (slotIndex < 0 || slotIndex >= 32) return;

        uint32_t currentMask = configItem.Get();

        bool isActive = (currentMask & (1u << slotIndex)) != 0;

        if (ImGui::Checkbox(label, &isActive))
        {
                if (isActive)
                        configItem.Set(currentMask | (1u << slotIndex));  // Flip bit to 1
                else
                        configItem.Set(currentMask & ~(1u << slotIndex)); // Flip bit to 0
        }
}

void DrawMaterialEditor()
{
        static TextEditor editor{};
        static int selected = -1;
        static int last_selected = -1;
        
        // CHANGED: Pulling straight from unified manager instance
        auto& materials = features::materials.GetMaterials();

        constexpr int TABLE_FLAGS = ImGuiTableFlags_Resizable | 
                                    ImGuiTableFlags_BordersInnerV |
                                    ImGuiTableFlags_SizingStretchSame;

        if (ImGui::BeginTable("##ChamsTable", 2, TABLE_FLAGS))
        {
                ImGui::TableSetupColumn("Materials", ImGuiTableColumnFlags_WidthStretch, 0.35f);
                ImGui::TableSetupColumn("Editor", ImGuiTableColumnFlags_WidthStretch, 0.65f);

                ImGui::TableNextRow();
        
                // Material List View
                ImGui::TableNextColumn();
                {
                        if (ImGui::BeginChild("##MatList"))
                        {
                                ImGui::Separator();
                                for (int i = 0; i < materials.size(); i++)
                                {
                                        if (!materials[i]->IsValidMat())
                                                continue;

                                        bool is_selected = (selected == i);
                                        if (ImGui::Selectable(materials[i]->GetDisplayName().c_str(), is_selected))
                                                selected = i;
                                }
                                ImGui::Separator();

                                // New Popup Handler
                                if (ImGui::BeginPopup("NewMaterialPopup"))
                                {
                                        static char text[50] = "chams/my_material"; // Recommend user specifies folder prefix
                                        ImGui::InputText("Material Path/Name", text, sizeof(text));

                                        if (ImGui::Button("Create"))
                                        {
                                                std::string vmt = "UnlitGeneric\n{\n\t$basetexture \"white\"\n}";
                                                std::shared_ptr<CustomMaterial> dummy;
                                                
                                                // CHANGED: Calling Unified AddMaterial
                                                if (features::materials.AddMaterial(text, vmt, dummy))
                                                {
                                                        selected = materials.size() - 1;
                                                        editor.SetText(vmt);
                                                }
                                                ImGui::CloseCurrentPopup();
                                        }
                                        ImGui::SameLine();
                                        if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
                                        ImGui::EndPopup();
                                        }

                                        if (ImGui::Button("+ New")) ImGui::OpenPopup("NewMaterialPopup");
                                        
                                        ImGui::SameLine();
                                        if (ImGui::Button("- Delete") && selected != -1)
                                        {
                                                auto& mat = materials[selected];
                                                if (mat->IsValidMat())
                                                {
                                                        const std::string& dispName = mat->GetDisplayName();
                                                        std::filesystem::path filePath = std::filesystem::path(MATERIAL_DIR) / (dispName + ".vmt");

                                                        if (std::filesystem::exists(filePath))
                                                                std::filesystem::remove(filePath);

                                                        uint32_t matHash = fnv::Hash(dispName.c_str());
                                                        if (config::chams::material.Get() == matHash)
                                                                config::chams::material.Set(0);
                                                        if (config::backtrack::material.Get() == matHash)
                                                                config::backtrack::material.Set(0);

                                                        // CHANGED: Call unified manager removal
                                                        features::materials.RemoveMaterial(dispName);
                                                        selected = -1;
                                                }
                                        }
                                }
                                ImGui::EndChild();
                        }

                // Code Editor View
                ImGui::TableNextColumn();
                {
                        if (ImGui::BeginChild("##MatEditor"))
                        {
                                if (selected != -1)
                                {
                                        auto& mat = materials[selected];

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
                                                mat->SetVMT(editor.GetText());
                                                mat->Refresh();
                                        }

                                        ImGui::SameLine();
                                        if (ImGui::Button("Get VMT")) editor.SetText(mat->GetVMT());

                                        if (mat->m_iSlotIndex != -1)
					{
						ToggleMaterialUsage(config::chams::material, mat->m_iSlotIndex, "Used for Chams");
						ImGui::SameLine();
						ToggleMaterialUsage(config::backtrack::material, mat->m_iSlotIndex, "Used for Backtrack");
					}

                                        float alpha = mat->GetAlpha();
                                        if (ImGui::SliderFloat("Alpha##Chams", &alpha, 0.0f, 1.0f))
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