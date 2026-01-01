#pragma once

#include "types.h"

class VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(Vector X, Vector Y, Vector Z) 
	{
		//Init(X,Y,Z);
		X.x = 0;
		X.y = 0;
		X.z = 0;

		Y.x = 0;
		Y.y = 0;
		Y.z = 0;

		Z.x = 0;
		Z.y = 0;
		Z.z = 0;
	}
public:
	explicit VectorAligned(const Vector &vOther) 
	{
		//Init(vOther.x, vOther.y, vOther.z);
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;
	}
	
	VectorAligned& operator=(const Vector &vOther)	
	{
		//Init(vOther.x, vOther.y, vOther.z);
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;
		return *this;
	}
	float w;	// this space is used anyway
};