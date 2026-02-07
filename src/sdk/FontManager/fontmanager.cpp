#include "fontmanager.h"

#include "../../sdk/definitions/ipanel.h"
#include "../../sdk/interfaces/interfaces.h"

FontManager g_FontManager;

int FontManager::CreateFont(const std::string& id, const std::string& fontName, int height, int weight, int flags)
{
	if (id.empty() || fontName.empty())
		return 0;

	auto it = m_Fonts.find(id);
	if (it != m_Fonts.end())
		return it->second;

	HFont font = interfaces::Surface->CreateFont();
	interfaces::Surface->SetFontGlyphSet(font, fontName.c_str(), height, weight, 0, 0, flags);

	m_Fonts[id] = font;
	return font;
}

int FontManager::GetFont(const std::string& id)
{
	auto it = m_Fonts.find(id);
	if (it == m_Fonts.end())
		return 0;

	return it->second;
}

int FontManager::SetFont(const std::string& id)
{
	auto it = m_Fonts.find(id);
	if (it == m_Fonts.end())
		return false;

	m_currentFontID = id;
	interfaces::Surface->DrawSetTextFont(it->second);
	return true;
}

const std::string& FontManager::GetCurrentFontID()
{
	return m_currentFontID;
}

const std::unordered_map<std::string, unsigned long>& FontManager::GetAllFonts()
{
	return m_Fonts;
}