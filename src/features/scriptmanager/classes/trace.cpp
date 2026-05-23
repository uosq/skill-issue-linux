#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/classes/entity.h"

void BindTrace(sol::state& lua)
{
	lua.new_usertype<csurface_t>
	(
		"Surface", sol::no_constructor,

		//fields
		"name", &csurface_t::name,
		"surfaceProps", &csurface_t::surfaceProps,
		"flags", &csurface_t::flags
	);

	lua.new_usertype<cplane_t>
	(
		"Plane", sol::no_constructor,

		"normal", &cplane_t::normal,
		"dist", &cplane_t::dist,
		"type", &cplane_t::type,
		"signbits", &cplane_t::signbits
	);

	lua.new_usertype<CGameTrace>
	(
		"Trace", sol::constructors<CGameTrace()>(),

		// fields
		"fractionleftsolid", &CGameTrace::fractionleftsolid,
		"surface", &CGameTrace::surface,
		"hitgroup", &CGameTrace::hitgroup,
		"physicsbone", &CGameTrace::physicsbone,
		"entity", &CGameTrace::m_pEnt,
		"hitbox", &CGameTrace::hitbox,
		"startpos", &CGameTrace::startpos,
		"endpos", &CGameTrace::endpos,
		"plane", &CGameTrace::plane,
		"fraction", &CGameTrace::fraction,
		"contents", &CGameTrace::contents,
		"dispFlags", &CGameTrace::dispFlags,
		"allsolid", &CGameTrace::allsolid,
		"startsolid", &CGameTrace::startsolid,

		// methods
		"DidHit", &CGameTrace::DidHit
	);
}