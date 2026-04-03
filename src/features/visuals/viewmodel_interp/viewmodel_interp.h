#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../settings/settings.h"

namespace ViewmodelInterp
{
	extern Vector m_vecOldAngle;

	void Init();
	void Run(Vector &angle);
}; // namespace ViewmodelInterp