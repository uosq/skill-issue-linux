#include "../../../thirdparty/sol3/sol.hpp"

#include "../../../sdk/definitions/types.h"

void BindVector(sol::state& lua)
{
	lua.new_usertype<Vec3>
	(
		"Vector3", sol::constructors<Vec3(), Vec3(float, float, float)>(),

		"x", &Vec3::x,
		"y", &Vec3::y,
		"z", &Vec3::z,

		sol::meta_function::addition, [](const Vec3& v, const Vec3& b)
		{
			return v + b;
		},

		sol::meta_function::subtraction, [](const Vec3& v, const Vec3& b)
		{
			return v - b;
		},

		sol::meta_function::multiplication, [](const Vec3& v, float scalar)
		{
			return v * scalar;
		},

		sol::meta_function::division, [](const Vec3& v, float scalar)
		{
			return v / scalar;
		},

		// -vec
		sol::meta_function::unary_minus, [](const Vec3& v)
		{
			return Vec3(-v.x, -v.y, -v.z);
		},

		sol::meta_function::equal_to, &Vec3::operator==,

		sol::meta_function::to_string, [](const Vec3& v)
		{
			return "Vector3(" + std::to_string(v.x) + ", " +
					    std::to_string(v.y) + ", " +
					    std::to_string(v.z) + ")";
		},

		sol::meta_function::length, &Vec3::Length,

		"Zero",		&Vec3::Zero,
		"Set",		&Vec3::Set,
		"To2D",		&Vec3::To2D,
		"Length",	&Vec3::Length,
		"LengthSqr",	&Vec3::LengthSqr,
		"Normalize",	&Vec3::Normalize,
		"Normalize2D",	&Vec3::Normalize2D,
		"Normalized",	&Vec3::Normalized,
		"Normalized2D",	&Vec3::Normalized2D,
		"Get2D",	&Vec3::Get2D,
		"Length2D",	&Vec3::Length2D,
		"Length2DSqr",	&Vec3::Length2DSqr,
		"DistTo",	&Vec3::DistTo,
		"DistTo2D",	&Vec3::DistTo2D,
		"DistToSqr",	&Vec3::DistToSqr,
		"DistTo2DSqr",	&Vec3::DistTo2DSqr,
		"Dot",		&Vec3::Dot,
		"Cross",	&Vec3::Cross,
		"IsZero",	&Vec3::IsZero,
		"ToAngle",	&Vec3::ToAngle,
		"FromAngle",	&Vec3::FromAngle
	);
}
