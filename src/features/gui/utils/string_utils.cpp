#include "string_utils.h"
#include <algorithm>

std::string ToLower(const std::string& str)
{
	// gotta make sure we have enough stack space lol
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
        return result;
}

bool ContainsInsensitive(const std::string &text, const std::string &search)
{
	// bro why can't it just be a fucking bool and return on a parameter???
	// fuck you c++
	return ToLower(text).find(ToLower(search)) != std::string::npos;
}