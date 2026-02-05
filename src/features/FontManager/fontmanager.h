#pragma once

#include <string>
#include <unordered_map>

class FontManager
{
private:
	std::unordered_map<std::string, int> m_Fonts;

public:
	FontManager()
	{
		m_Fonts.reserve(10);
	}

	int CreateFont(const std::string& id, const std::string& fontName, int height, int weight, int flags);
	int GetFont(const std::string& id);
};

extern FontManager g_FontManager;