#include "../../../thirdparty/sol3/sol.hpp"

#include "../../network/network.h"

void BindNetwork(sol::state& lua)
{
	auto network = lua.create_table("network");

	network["get"] = [](const std::string& url) -> std::string
	{
		bool success = false;
		std::string result = features::network.request(url, success);
		return result;
	};
}