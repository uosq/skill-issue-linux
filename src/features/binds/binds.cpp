#include "binds.h"

#include "../../sdk/interfaces/interfaces.h"

bool Binds::IsKeyDown(const Hotkey *hk)
{
	if (hk->m_iType == InputType::None)
		return false;

	return ImGui::IsKeyDown((ImGuiKey)hk->m_iKey);
}

int Binds::GetPressedKey()
{
	if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft, false))
		return ImGuiKey_MouseLeft;
	if (ImGui::IsKeyPressed(ImGuiKey_MouseRight, false))
		return ImGuiKey_MouseRight;
	if (ImGui::IsKeyPressed(ImGuiKey_MouseMiddle, false))
		return ImGuiKey_MouseMiddle;
	if (ImGui::IsKeyPressed(ImGuiKey_MouseX1, false))
		return ImGuiKey_MouseX1;
	if (ImGui::IsKeyPressed(ImGuiKey_MouseX2, false))
		return ImGuiKey_MouseX2;

	for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++)
	{
		if (ImGui::IsKeyPressed((ImGuiKey)i, false))
			return i;
	}

	return 0;
}

const char *Binds::GetKeyName(const Hotkey *hk)
{
	if (hk->m_iType == InputType::None)
		return "None";

	return ImGui::GetKeyName((ImGuiKey)hk->m_iKey);
}

void Binds::RegisterHotkey(Hotkey* hk)
{
	hk->next = GetHead();
	GetHead() = hk;
}

bool Binds::IsActive(const Hotkey *hk) const
{
	return hk->m_bState;
}

void Binds::Update()
{
	for (Hotkey* hk = GetHead(); hk != nullptr; hk = hk->next)
	{
		bool currentlyPressed = IsKeyDown(hk);
		bool wasPressed       = hk->m_bIsPressed;
		hk->m_bIsPressed      = currentlyPressed;

		switch (hk->m_iMode)
		{
		case HotkeyMode::Hold:    hk->m_bState = currentlyPressed; break;
		case HotkeyMode::Toggle:  if (currentlyPressed && !wasPressed) hk->m_bState = !hk->m_bState; break;
		case HotkeyMode::HoldOff: hk->m_bState = !currentlyPressed; break;
		case HotkeyMode::Always:  hk->m_bState = true; break;
		default:                  hk->m_bState = false; break;
		}
	}
}

bool Binds::RenderHotkey(const char *label, Hotkey *hk)
{
	bool changed = false;

	ImGui::PushID(hk);

	const char *keyName = hk->m_bCapturing ? "Press key..." : GetKeyName(hk);

	if (ImGui::Button(keyName, ImVec2(120, 0)))
		hk->m_bCapturing = true;

	if (ImGui::BeginPopup("HotkeyModeMenu"))
	{
		constexpr const char *modes[] = {"Off", "Hold", "Toggle", "Hold Off", "Always"};

		for (int i = 0; i < IM_ARRAYSIZE(modes); i++)
		{
			bool selected = (int)hk->m_iMode == i;

			if (ImGui::Selectable(modes[i], selected))
			{
				hk->m_iMode = (HotkeyMode)i;
				changed	    = true;
			}

			if (selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndPopup();
	}

	ImGui::SameLine();
	if (ImGui::Button("+"))
		ImGui::OpenPopup("HotkeyModeMenu");

	ImGui::SameLine();
	ImGui::TextUnformatted(label);

	if (hk->m_bCapturing)
	{
		int key = GetPressedKey();

		if (key == ImGuiKey_Escape)
		{
			hk->m_bCapturing = false;
		}
		else if (key == ImGuiKey_Delete)
		{
			hk->m_iType	 = InputType::None;
			hk->m_iKey	 = 0;

			hk->m_bCapturing = false;
			changed		 = true;
		}
		else if (key != 0)
		{
			hk->m_iType	 = InputType::VirtualKey;
			hk->m_iKey	 = key;

			hk->m_bCapturing = false;
			changed		 = true;
		}
	}

	ImGui::PopID();

	return changed;
}

const char *Binds::GetModeName(HotkeyMode mode)
{
	switch (mode)
	{
	case HotkeyMode::Off:
		return "Off";
	case HotkeyMode::Hold:
		return "Hold";
	case HotkeyMode::Toggle:
		return "Toggle";
	case HotkeyMode::HoldOff:
		return "Hold Off";
	case HotkeyMode::Always:
		return "Always On";
	default:
		return "None";
	}
}

void Binds::DrawWindow(bool bMenuOpen)
{
	if (interfaces::Engine->IsTakingScreenshot())
		return;

	ImGui::SetNextWindowSizeConstraints(ImVec2{150.0f, 0.0f}, ImVec2(FLT_MAX, FLT_MAX));

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!bMenuOpen) flags |= ImGuiWindowFlags_NoMove;

	ImGui::Begin("Binds", nullptr, flags);

	// Loop through the intrusive linked list
	for (Hotkey* hk = GetHead(); hk != nullptr; hk = hk->next)
	{
		if (hk->m_iType == InputType::None)
			continue;

		const HotkeyMode &iMode = hk->m_iMode;
		const char* strModeName = GetModeName(iMode);
		const char* strKeyName  = GetKeyName(hk);

		if (hk->m_iMode != HotkeyMode::Off)
		{
			ImVec4 color = ImGui::ColorConvertU32ToFloat4(hk->m_bState ? IM_COL32(100, 255, 100, 255)
										     : IM_COL32(255, 255, 255, 255));
			ImGui::TextColored(color, "%s - %s [%s]", strKeyName, hk->m_strName.data(), strModeName);
		}
		else
		{
			ImGui::TextDisabled("%s - %s [%s]", strKeyName, hk->m_strName.data(), strModeName);
		}
	}

	ImGui::End();
}

bool Binds::IsEnabled(const Hotkey *hk) const
{
	return hk->m_iType == InputType::VirtualKey && hk->m_iMode != HotkeyMode::Off;
}