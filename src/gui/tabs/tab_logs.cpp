#include "tab_logs.h"

#include "../../imgui/imgui.h"
#include "../../features/logs/logs.h"

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