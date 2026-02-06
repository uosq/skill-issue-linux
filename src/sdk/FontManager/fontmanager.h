#pragma once

#include <string>
#include <unordered_map>

class FontManager
{
private:
	std::unordered_map<std::string, unsigned long> m_Fonts;
	std::string m_currentFontID;

public:
	FontManager()
	{
		m_Fonts.reserve(10);
		m_currentFontID = "";
	}

	~FontManager()
	{
		m_Fonts.clear();
		m_currentFontID.clear();
	}

	int CreateFont(const std::string& id, const std::string& fontName, int height, int weight, int flags);
	int GetFont(const std::string& id);
	int SetFont(const std::string& id);
	const std::string& GetCurrentFontID();
	const std::unordered_map<std::string, unsigned long>& GetAllFonts();
};

extern FontManager g_FontManager;