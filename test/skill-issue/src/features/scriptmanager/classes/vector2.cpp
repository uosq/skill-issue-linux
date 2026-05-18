#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/types.h"

void BindVector2(sol::state& lua)
{
	lua.new_usertype<Vec2>
	(
		"Vector2", sol::constructors<Vec2()>(),

		"x", &Vec2::x,
		"7", &Vec2::y
	);
}