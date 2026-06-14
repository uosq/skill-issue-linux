#include "../../../thirdparty/sol3/sol.hpp"

#include "../../configmanager/configmgr.h"

void BindConfig(sol::state& lua)
{
	auto config = lua.create_table("config");

	config["load_file"] = [](const std::string& filepath)
	{
		if (filepath.length() == 0)
			return;

		features::configs.Load(filepath);
	};

	config["save_file"] = [](const std::string& filepath)
	{
		if (filepath.length() == 0)
			return;

		
	};
}