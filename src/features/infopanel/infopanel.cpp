#include "infopanel.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#include "../aimbot/aimbot.h"
#include "../spectators/spectators.h"
#include "../warp/warp.h"
#include "../radar/radar.h"

static void DrawColoredText(const char* text, bool state)
{
	ImVec4 col = state ? ImVec4(100, 255, 100, 255) : ImVec4(255, 255, 255, 255);
	ImGui::TextColored(col, "%s: %s", text, state ? "ON" : "OFF");
}

template <typename... text>
static void DrawColoredTextEx(const char* fmt, text... txt)
{
	ImGui::TextColored(ImVec4(255, 255, 255, 255), fmt, txt...);
}

void InfoPanel::OnImGui(bool bMenuOpen)
{
	if (!Settings::Misc.infopanel)
		return;

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!bMenuOpen)
		flags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Information Panel", nullptr, flags))
	{
		DrawColoredText("Aimbot", Aimbot::IsRunning());
		DrawColoredText("ESP", Settings::ESP.enabled);
		DrawColoredText("Chams", Settings::ESP.chams);
		DrawColoredText("Autostrafe", Settings::Misc.autostrafe);
		DrawColoredText("Glow", Settings::ESP.blur > 0);
		DrawColoredText("Spectated", Spectators::IsLocalPlayerSpectated());

		DrawColoredText("Warp", Settings::AntiAim.warp_key->IsEnabled());
		DrawColoredTextEx("Warp Amount: %i", Warp::m_iShiftAmount);
		DrawColoredTextEx("Warp Ticks: %i", Warp::m_iStoredTicks);

		DrawColoredText("Radar", Settings::Radar.enabled);
		DrawColoredTextEx("Radar Range: %i", Radar::m_iRange);
		DrawColoredTextEx("Radar Radius: %.3f", Radar::m_flRadius);
	}
	ImGui::End();
}