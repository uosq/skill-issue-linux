#pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

namespace ViewmodelInterp
{
	extern Vector m_vecOldAngle;

	void Init();
	void Run(Vector& angle);
};