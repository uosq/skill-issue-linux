#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../imgui/imgui.h"

enum class HotkeyMode
{
	Off,
	Hold,
	Toggle,
	HoldOff,
	Always
};

enum class InputType
{
	None,
	VirtualKey
};

struct Hotkey
{
	std::string m_strName;

	InputType m_iType  = InputType::None;
	HotkeyMode m_iMode = HotkeyMode::Off;

	int m_iKey	   = 0;

	bool m_bState	   = false;
	bool m_bIsPressed  = false;

	bool m_bCapturing  = false;

	bool IsActive()
	{
		return m_bState;
	}

	bool IsEnabled()
	{
		return m_iType == InputType::VirtualKey && m_iMode != HotkeyMode::Off;
	}
};

class Binds
{
      private:
	std::vector<std::unique_ptr<Hotkey>> m_hotkeys;
	bool IsKeyDown(const Hotkey *hk);
	int GetPressedKey();
	const char *GetKeyName(const Hotkey *hk);

      public:
	Hotkey *RegisterHotkey(const char *name);
	const std::vector<std::unique_ptr<Hotkey>> &GetHotkeys() const;
	bool IsActive(const Hotkey *hk) const;
	bool IsEnabled(const Hotkey *hk) const;
	void Update();
	bool RenderHotkey(const char *label, Hotkey *hk);

	const char *GetModeName(HotkeyMode mode);
	void DrawWindow(bool bMenuOpen);
};

extern Binds gBinds;