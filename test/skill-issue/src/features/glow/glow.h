#pragma once

#include <unordered_set>
#include <vector>

#include "../../sdk/MaterialManager/materialmanager.h"
#include "../../sdk/definitions/itexture.h"
#include "../../sdk/definitions/texture_group_names.h"
#include "../../sdk/helpers/helper.h"

#include "../../features/esp/esp.h"
#include "../feature.h"

struct GlowMaterials
{
	IMaterial *glowColor	   = nullptr;
	IMaterial *haloAddToScreen = nullptr;
	IMaterial *blurX	   = nullptr;
	IMaterial *blurY	   = nullptr;
	ITexture *pRtFullFrame	   = nullptr;
	ITexture *glowBuffer1	   = nullptr;
	ITexture *glowBuffer2	   = nullptr;
};

class Glow
{
public:
	void Init();
	bool ShouldHide(int entindex);
	void DrawEntities();
	void GetEntities();

	// Call in DoPostScreenSpaceEffects
	void Run();

	bool IsRunning();
private:
	GlowMaterials m_Materials{};
	std::vector<CBaseEntity *> glowEnts{};
	std::unordered_set<int> m_Entities{};
	bool m_bRunning{false};
	bool m_bInitialized{false};
};

DECLARE_FEATURE(Glow, glow)