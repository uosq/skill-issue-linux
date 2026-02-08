#pragma once

#include "../../sdk/MaterialManager/materialmanager.h"
#include "../../sdk/definitions/texture_group_names.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/definitions/itexture.h"
#include "../../features/esp/esp.h"
#include <unordered_set>
#include <vector>

struct GlowMaterials
{
	IMaterial* glowColor = nullptr;
	IMaterial* haloAddToScreen = nullptr;
	IMaterial* blurX = nullptr;
	IMaterial* blurY = nullptr;
	ITexture* pRtFullFrame = nullptr;
	ITexture* glowBuffer1 = nullptr;
	ITexture* glowBuffer2 = nullptr;
};

namespace Glow
{
	extern GlowMaterials m_Materials;
	extern std::vector<CBaseEntity*> glowEnts;
	extern std::unordered_set<int> m_Entities;
	extern bool m_bRunning;
	extern bool m_bInitialized;

	void Init();
	bool ShouldHide(int entindex);
	void DrawEntities();
	void GetEntities();

	// Call in DoPostScreenSpaceEffects
	void Run();
}