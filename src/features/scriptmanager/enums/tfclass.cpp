#include "../../../sol3/sol.hpp"
#include "../../../sdk/defs.h"

void BindTFClass(sol::state& lua)
{
	lua.new_enum
	(
		"TFClass",
		"Scout", TF_CLASS_SCOUT,
		"Sniper", TF_CLASS_SNIPER,
		"Soldier", TF_CLASS_SOLDIER,
		"Demoman", TF_CLASS_DEMOMAN,
		"Medic", TF_CLASS_MEDIC,
		"Heavyweapons", TF_CLASS_HEAVYWEAPONS,
		"Pyro", TF_CLASS_PYRO,
		"Spy", TF_CLASS_SPY,
		"Engineer", TF_CLASS_ENGINEER
	);

	lua.set("TF_CLASS_SCOUT", TF_CLASS_SCOUT);
	lua.set("TF_CLASS_SNIPER", TF_CLASS_SNIPER);
	lua.set("TF_CLASS_SOLDIER", TF_CLASS_SOLDIER);
	lua.set("TF_CLASS_DEMOMAN", TF_CLASS_DEMOMAN);
	lua.set("TF_CLASS_MEDIC", TF_CLASS_MEDIC);
	lua.set("TF_CLASS_HEAVYWEAPONS", TF_CLASS_HEAVYWEAPONS);
	lua.set("TF_CLASS_PYRO", TF_CLASS_PYRO);
	lua.set("TF_CLASS_SPY", TF_CLASS_SPY);
	lua.set("TF_CLASS_ENGINEER", TF_CLASS_ENGINEER);
}