#pragma once

#include "../../vtables.h"
#include "../definitions/cbaseanimating.h"

class CEconEntity : public CBaseAnimating
{
      public:
	NETVAR(m_iItemDefinitionIndex, "CEconEntity->m_iItemDefinitionIndex", int);
	void UpdateAttachmentModels()
	{
		return vtable::call<213, void>(this);
	}
};