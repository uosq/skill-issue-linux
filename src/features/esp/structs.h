#pragma once

#include <string>
#include "../../sdk/definitions/vector.h"

struct ESP_Data
{
	std::string name;
	int health;
	int maxhealth;
	int buffhealth;
	float width, height;

	Vec2 top;
	Vec2 bottom;
};

enum class ESP_ALIGNMENT
{
	INVALID = -1, 		// anythng equal or below this is invalid
	LEFT = 0, RIGHT,
	TOP, BOTTOM,
	MAX 			// anythng equal or below this is invalid
};

struct ESPContext
{
	float topOffset = 0.0f;
	float bottomOffset = 0.0f;
	float verticalLeftOffset = 0.0f;
	float verticalRightOffset = 0.0f;
	/*Vector absMins;
	Vector absMaxs;*/

	ESPContext()
	{
		/*absMins = {0, 0};
		absMaxs = {0, 0};*/

		topOffset = 0.0f;
		bottomOffset = 0.0f;
		verticalLeftOffset = 0.0f;
		verticalRightOffset = 0.0f;
	}
};