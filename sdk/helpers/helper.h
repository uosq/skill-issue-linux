#pragma once

#include "../interfaces/interfaces.h"
#include "../definitions/color.h"
#include "../classes/player.h"
#include "fonts.h"

#include <cstring>
#include <string>

namespace helper
{
	namespace draw
	{

		inline HFont CreateFont(const char* name, int height, int weight, int flags = 0)
		{
			return fontManager.CreateFont(name, height, weight, flags);
		}

		inline void SetFont(HFont font)
		{
			fontManager.SetCurrentFont(font);
			interfaces::surface->DrawSetTextFont(font);
		}

		inline HFont GetCurrentFont()
		{
			return fontManager.GetCurrentFont();
		}

		inline void FilledRect(int x0, int y0, int x1, int y1)
		{
			interfaces::surface->DrawFilledRect(x0, y0, x1, y1);
		}

		inline void SetColor(Color col)
		{
			interfaces::surface->DrawSetColor(col);
		}

		inline void SetColor(int r, int g, int b, int a = 255)
		{
			interfaces::surface->DrawSetColor(r, g, b, a);
		}

		inline void Text(int x, int y, Color color, std::string text)
		{
			// convert our string to wide string
			// fuck my life
			std::wstring wtext(text.begin(), text.end());
			interfaces::surface->DrawSetTextPos(x, y);
			interfaces::surface->DrawSetTextColor(color);
			interfaces::surface->DrawPrintText(wtext.c_str(), wtext.length());
		}

		inline void GetMousePosition(int &x, int &y)
		{
			interfaces::surface->SurfaceGetCursorPos(x, y);
		}

		inline void GetTextSize(std::string text, int &width, int &height)
		{
			std::wstring wtext(text.begin(), text.end());
			interfaces::surface->GetTextSize(fontManager.GetCurrentFont(), wtext.c_str(), width, height);
		}

		inline std::wstring ConvertStringToWChar(std::string text)
		{
			return std::wstring(text.begin(), text.end());
		}
	};

	namespace engine
	{
		inline bool IsGameUIVisible()
		{
			return interfaces::enginevgui->IsGameUIVisible();
		}

		inline int GetMaxClients()
		{
			return interfaces::enginetool->GetMaxClients();
		}

		inline bool IsTakingScreenshot()
		{
			return interfaces::engine->IsTakingScreenshot();
		}

		inline bool IsConsoleVisible()
		{
			return interfaces::engine->Con_IsVisible();
		}

		inline Player* GetLocalPlayer()
		{
			int index = interfaces::engine->GetLocalPlayer();
			return (Player*)interfaces::entitylist->GetClientEntity(index);
		}
	};

	namespace console
	{
		inline void Print(const char* text)
		{
			interfaces::vstdlib->ConsolePrintf(text);
		}

		inline void ColoredPrint(const char* text, Color_t color)
		{
			interfaces::vstdlib->ConsoleColorPrintf(color, text);
		}
	}
};