#pragma once

#include "../../interfaces/interfaces.h"
#include "../../definitions/color.h"
#include "../../classes/player.h"
#include "../../classes/weaponbase.h"
#include "../math.h"
#include "../../FontManager/fontmanager.h"

#include <cmath>
#include <csignal>
#include <cstring>
#include <string>

namespace helper
{
	namespace draw
	{
		inline HFont CreateFont(const std::string& id, const std::string& name, int height, int weight, int flags = 0)
		{
			return g_FontManager.CreateFont(id, name, height, weight, flags);
		}

		inline void SetFont(const std::string& id)
		{
			g_FontManager.SetFont(id);
		}

		inline void FilledRect(int x0, int y0, int x1, int y1)
		{
			interfaces::Surface->DrawFilledRect(x0, y0, x1, y1);
		}

		inline void OutlinedRect(int x0, int y0, int x1, int y1)
		{
			interfaces::Surface->DrawOutlinedRect(x0, y0, x1, y1);
		}

		inline void SetColor(Color col)
		{
			interfaces::Surface->DrawSetColor(col);
		}

		inline void SetColor(int r, int g, int b, int a = 255)
		{
			interfaces::Surface->DrawSetColor(r, g, b, a);
		}

		inline void Text(int x, int y, Color color, std::string text)
		{
			// convert our string to wide string
			// fuck my life
			std::wstring wtext(text.begin(), text.end());
			interfaces::Surface->DrawSetTextPos(x, y);
			interfaces::Surface->DrawSetTextColor(color);
			interfaces::Surface->DrawPrintText(wtext.c_str(), wtext.length());
		}

		inline void TextShadow(int x, int y, Color color, std::string text)
		{
			// convert our string to wide string
			// fuck my life
			std::wstring wtext(text.begin(), text.end());
			Color black = {0, 0, 0, 255};

			interfaces::Surface->DrawSetTextPos(x + 1, y + 1);
			interfaces::Surface->DrawSetTextColor(black);
			interfaces::Surface->DrawPrintText(wtext.c_str(), wtext.length());

			interfaces::Surface->DrawSetTextPos(x, y);
			interfaces::Surface->DrawSetTextColor(color);
			interfaces::Surface->DrawPrintText(wtext.c_str(), wtext.length());
		}

		inline void GetMousePosition(int &x, int &y)
		{
			interfaces::Surface->SurfaceGetCursorPos(x, y);
		}

		inline void GetTextSize(std::string text, int &width, int &height)
		{
			std::wstring wtext(text.begin(), text.end());

			std::string currentFontID = g_FontManager.GetCurrentFontID();
			int font = g_FontManager.GetFont(currentFontID);

			interfaces::Surface->GetTextSize(font, wtext.c_str(), width, height);
		}

		inline std::wstring ConvertStringToWChar(std::string text)
		{
			return std::wstring(text.begin(), text.end());
		}

		inline void GetScreenSize(int &w, int &h)
		{
			w = interfaces::ClientDLL->GetScreenWidth();
			h = interfaces::ClientDLL->GetScreenHeight();
		}
	}
};