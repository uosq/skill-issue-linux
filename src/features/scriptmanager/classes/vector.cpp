#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/types.h"

void BindVector(sol::state& lua)
{
	lua.new_usertype<Vec3>
	(
		"Vector3", sol::constructors<Vec3()>(),

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
		}
	);
}