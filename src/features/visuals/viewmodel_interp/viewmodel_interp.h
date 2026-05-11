#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../settings/settings.h"

#include "../../feature.h"

class ViewmodelInterp
{
public:
	void Init();
	void Run(Vector &angle);

private:
	Vector m_vecOldAngle{};
};

DECLARE_FEATURE(ViewmodelInterp, viewmodel_interp)