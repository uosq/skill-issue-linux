#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/types.h"

void BindVector2(sol::state& lua)
{
	lua.new_usertype<Vec2>
	(
		"Vector2", sol::constructors<Vec2(), Vec2(float, float)>(),

		"x", &Vec2::x,
		"y", &Vec2::y,

		sol::meta_function::length, &Vec2::Length,

		"Zero", &Vec2::Zero,
		"Set", &Vec2::Set,
		"Length", &Vec2::Length,
		"LengthSqr", &Vec2::LengthSqr,
		"DistTo", &Vec2::DistTo,
		"DistToSqr", &Vec2::DistToSqr,
		"Dot", &Vec2::Dot,
		"IsZero", &Vec2::IsZero
	);
}