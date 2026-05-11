#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/cusercmd.h"

void BindUserCmd(sol::state& lua)
{
	lua.new_usertype<CUserCmd>
	(
		"UserCmd", sol::constructors<CUserCmd()>(),

		"command_number", &CUserCmd::command_number,
		"tick_count", &CUserCmd::tick_count,
		"viewangles", &CUserCmd::viewangles,
		"forwardmove", &CUserCmd::forwardmove,
		"sidemove", &CUserCmd::sidemove,
		"upmove", &CUserCmd::upmove,
		"buttons", &CUserCmd::buttons,
		"impulse", &CUserCmd::impulse,
		"weaponselect", &CUserCmd::weaponselect,
		"weaponsubtype", &CUserCmd::weaponsubtype,
		"random_seed", &CUserCmd::random_seed,
		"mousedx", &CUserCmd::mousedx,
		"mousedy", &CUserCmd::mousedy,
		"hasbeenpredicted", &CUserCmd::hasbeenpredicted
	);
}