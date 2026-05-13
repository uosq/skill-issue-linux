#include "../../../sdk/classes/player.h"
#include "../../../sol3/sol.hpp"

void BindPlayerFlags(sol::state& lua)
{
	lua.set
	(
		"FL_ONGROUND", FL_ONGROUND,
		"FL_DUCKING", FL_DUCKING,
		"FL_WATERJUMP", FL_WATERJUMP,
		"FL_ONTRAIN", FL_ONTRAIN,
		"FL_INRAIN", FL_INRAIN,
		"FL_FROZEN", FL_FROZEN,
		"FL_ATCONTROLS", FL_ATCONTROLS,
		"FL_CLIENT", FL_CLIENT,
		"FL_FAKECLIENT", FL_FAKECLIENT,
		"FL_INWATER", FL_INWATER
	);
}