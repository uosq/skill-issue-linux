#pragma once

#include <string>
#include <unordered_map>

namespace FontManager
{
	extern std::unordered_map<std::string, unsigned long> m_Fonts;
	extern std::string m_currentFontID;

	void Init(void);
	void Unitialize(void);

	int CreateFont(const std::string& id, const std::string& fontName, int height, int weight, int flags);
	int GetFont(const std::string& id);
	int SetFont(const std::string& id);
	const std::string& GetCurrentFontID();
	const std::unordered_map<std::string, unsigned long>& GetAllFonts();
};
