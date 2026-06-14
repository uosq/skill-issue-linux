#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"

#include "../../config/config.h"
#include "../../feature.h"

BEGIN_CONFIG(viewmodel_interp)

	ADD_CONFIG(enabled, "viewmodel interp enabled", 0.0f)

END_CONFIG()

class ViewmodelInterp
{
public:
	void Init();
	void Run(Vector &angle);

private:
	Vector m_vecOldAngle{};
};

DECLARE_FEATURE(ViewmodelInterp, viewmodel_interp)