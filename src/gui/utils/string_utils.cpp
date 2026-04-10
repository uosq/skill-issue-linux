#include "string_utils.h"
#include <algorithm>

std::string ToLower(std::string str)
{
	// wtf is this syntax
	// why the fuck do we need to specify the begin, end and then
	// the fucking begin again?? fuck you C++
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
	return str;
}

bool ContainsInsensitive(const std::string &text, const std::string &search)
{
	// bro why can't it just be a fucking bool and return on a parameter???
	// fuck you c++
	return ToLower(text).find(ToLower(search)) != std::string::npos;
}