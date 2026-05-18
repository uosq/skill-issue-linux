#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/cusercmd.h"

void Bind_IN_Buttons(sol::state& lua)
{
	lua.set("IN_ATTACK", IN_ATTACK);
	lua.set("IN_JUMP", IN_JUMP);
	lua.set("IN_DUCK", IN_DUCK);
	lua.set("IN_FORWARD", IN_FORWARD);
	lua.set("IN_BACK", IN_BACK);
	lua.set("IN_USE", IN_USE);
	lua.set("IN_CANCEL", IN_CANCEL);
	lua.set("IN_LEFT", IN_LEFT);
	lua.set("IN_RIGHT", IN_RIGHT);
	lua.set("IN_MOVELEFT", IN_MOVELEFT);
	lua.set("IN_MOVERIGHT", IN_MOVERIGHT);
	lua.set("IN_ATTACK2", IN_ATTACK2);
	lua.set("IN_RUN", IN_RUN);
	lua.set("IN_RELOAD", IN_RELOAD);
	lua.set("IN_ALT1", IN_ALT1);
	lua.set("IN_ALT2", IN_ALT2);
	lua.set("IN_SCORE", IN_SCORE);
	lua.set("IN_SPEED", IN_SPEED);
	lua.set("IN_WALK", IN_WALK);
	lua.set("IN_ZOOM", IN_ZOOM);
	lua.set("IN_WEAPON1", IN_WEAPON1);
	lua.set("IN_WEAPON2", IN_WEAPON2);
	lua.set("IN_BULLRUSH", IN_BULLRUSH);
	lua.set("IN_GRENADE1", IN_GRENADE1);
	lua.set("IN_GRENADE2", IN_GRENADE2);
	lua.set("IN_ATTACK3", IN_ATTACK3);
}