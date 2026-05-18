#pragma once

#include "../../classes/player.h"
#include "../../classes/weaponbase.h"
#include "../../definitions/color.h"
#include "../../interfaces/interfaces.h"
#include "../math.h"

#include <cmath>
#include <csignal>
#include <cstring>
#include <string>

namespace helper
{
	namespace draw
	{
		inline HFont CreateFont(const std::string &id, const std::string &name, int height, int weight,
					int flags = 0)
		{
			HFont font = interfaces::Surface->CreateFont();
			interfaces::Surface->SetFontGlyphSet(font, name.c_str(), height, weight, 0, 0, flags);
			return font;
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

		inline std::wstring ConvertStringToWChar(std::string text)
		{
			return std::wstring(text.begin(), text.end());
		}

		inline void GetScreenSize(int &w, int &h)
		{
			w = interfaces::ClientDLL->GetScreenWidth();
			h = interfaces::ClientDLL->GetScreenHeight();
		}
	} // namespace draw
}; // namespace helper