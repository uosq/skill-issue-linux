#include "fontmanager.h"

#include "../../sdk/definitions/ipanel.h"
#include "../../sdk/interfaces/interfaces.h"
#include <utility>

int FontManager::CreateFont(const std::string& id, const std::string& fontName, int height, int weight, int flags)
{
	HFont font = interfaces::Surface->CreateFont();
	interfaces::Surface->SetFontGlyphSet(font, fontName.c_str(), height, weight, 0, 0, flags);

	m_Fonts.emplace(std::pair<std::string, HFont>(id, font));
	return font;
}

int FontManager::GetFont(const std::string& id)
{
	auto it = m_Fonts.find(id);
	if (it == m_Fonts.end())
		return 0;

	return it->second;
}

FontManager g_FontManager;