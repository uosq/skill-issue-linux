#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "../../thirdparty/imgui/imgui.h"
#include "../feature.h"

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
	std::string_view m_strName;

	InputType m_iType  = InputType::None;
	HotkeyMode m_iMode = HotkeyMode::Off;

	int m_iKey	   = 0;

	bool m_bState	   = false;
	bool m_bIsPressed  = false;

	bool m_bCapturing  = false;

	Hotkey* next       = nullptr;

	bool IsActive() { return m_bState;}
	bool IsEnabled() { return m_iType == InputType::VirtualKey && m_iMode != HotkeyMode::Off; }
};

class Binds
{
public:
	static void RegisterHotkey(Hotkey* hk);

	bool IsActive(const Hotkey *hk) const;
	bool IsEnabled(const Hotkey *hk) const;
	void Update();
	bool RenderHotkey(const char *label, Hotkey *hk);

	const char *GetModeName(HotkeyMode mode);
	void DrawWindow(bool bMenuOpen);

	static Hotkey*& GetHead()
	{
		static Hotkey* head = nullptr;
		return head;
	}

private:
	bool IsKeyDown(const Hotkey *hk);
	int GetPressedKey();
	const char *GetKeyName(const Hotkey *hk);
};

DECLARE_FEATURE(Binds, binds)