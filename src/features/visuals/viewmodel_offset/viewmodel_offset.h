#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"

#include "../../config/config.h"
#include "../../feature.h"

BEGIN_CONFIG(viewmodel_offset)

	ADD_CONFIG(offset, "viewmodel offset offset", (std::array<float, 3>{}))

END_CONFIG()

class ViewmodelOffset
{
public:
	void Run(Vector &position, Vector &angle);
};

DECLARE_FEATURE(ViewmodelOffset, viewmodel_offset)