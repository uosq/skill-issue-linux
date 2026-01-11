#pragma once

#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/helpers/helper.h"
#include "../../features/esp/esp.h"
#include <vector>

namespace Chams
{
	static bool m_bMatLoaded = false;
	static IMaterial* m_mFlatMat = nullptr;
	static std::unordered_map<int, bool> m_DrawEnts;

	static bool Init()
	{
		if (m_mFlatMat == nullptr)
		{
			m_mFlatMat = helper::material::CreateMaterial("CoolFlatMat",
				"UnlitGeneric\n"
				"{\n"
				"	$basetexture \"white\"\n"
				"}"
			);
			m_bMatLoaded = true;
			helper::console::Print("Material vapooo created!\n");
		}

		return m_bMatLoaded;
	}

	static bool ShouldHide(int entindex)
	{
		return m_DrawEnts.find(entindex) != m_DrawEnts.end();
	}

	static void DrawPlayers()
	{
		m_DrawEnts.clear();

		float savedColor[3], savedBlend;
		savedBlend = interfaces::RenderView->GetBlend();

		interfaces::RenderView->GetColorModulation(savedColor);
		interfaces::RenderView->SetBlend(0.5f);
		interfaces::ModelRender->ForcedMaterialOverride(m_mFlatMat);

		for (int i = 1; i <= helper::engine::GetMaxClients(); i++)
		{
			CBaseEntity* baseEntity = static_cast<CBaseEntity*>(interfaces::EntityList->GetClientEntity(i));
			if (baseEntity == nullptr)
				continue;

			if (!baseEntity->IsPlayer())
				continue;

			if (baseEntity->IsDormant() || !baseEntity->ShouldDraw())
				continue;

			CTFPlayer* player = static_cast<CTFPlayer*>(baseEntity);
			if (player == nullptr)
				continue;

			if (!player->IsAlive())
				continue;
			
			Color color = ESP::GetPlayerColor(baseEntity);
			float flColor[3] = {color.r()/255.0f, color.g()/255.0f, color.b()/255.0f};

			interfaces::RenderView->SetColorModulation(flColor);
			player->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
		}

		interfaces::ModelRender->ForcedMaterialOverride(nullptr);
		interfaces::RenderView->SetColorModulation(savedColor);
		interfaces::RenderView->SetBlend(savedBlend);
	}

	static void Run()
	{
		if (!settings.esp.chams)
			return;

		Init();
		DrawPlayers();
	}
}