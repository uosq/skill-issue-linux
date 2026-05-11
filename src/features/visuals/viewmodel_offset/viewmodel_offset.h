#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../settings/settings.h"

#include "../../feature.h"

class ViewmodelOffset
{
public:
	void Run(Vector &position, Vector &angle);
};

DECLARE_FEATURE(ViewmodelOffset, viewmodel_offset)