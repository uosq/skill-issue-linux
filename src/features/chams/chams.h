#pragma once

#include "../../sdk/MaterialManager/materialmanager.h"
#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/helpers/helper.h"
#include "../../features/esp/esp.h"
#include "../entitylist/entitylist.h"
#include <unordered_set>
#include <vector>

namespace Chams
{
	extern bool m_bMatLoaded;
	extern IMaterial* m_mFlatMat;
	extern bool m_bRunning;
	extern std::unordered_set<int> m_Entities;

	bool Init();
	bool ShouldHide(int entindex);
	void DrawPlayers();
	void Run();
}