#pragma once

#include "types.h"

inline void Vector4DCopy(Vector4D const &src, Vector4D &dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
	dst.w = src.w;
}