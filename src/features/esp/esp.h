#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/definitions/eteam.h"

#include "../entitylist/entitylist.h"
#include "../../settings/settings.h"

#include "elements/BaseElement.h"
#include "elements/LuaElement.h"
#include "structs.h"

#include "esp_utils.h"

namespace ESP
{
	extern std::vector<std::unique_ptr<IBaseElement>> m_builtinElements;
	extern std::vector<std::unique_ptr<LuaElement>> m_luaElements;

	void PaintBox(Color color, const ESP_Data& data);

	bool GetData(const EntityListEntry& entry, ESP_Data& out);

	void Init();
	void Run(CTFPlayer* pLocal);
};