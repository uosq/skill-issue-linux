#pragma once

#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/helpers/helper.h"
#include "../../features/esp/esp.h"
#include "../entitylist/entitylist.h"
#include <vector>

namespace Chams
{
	static bool m_bMatLoaded = false;
	static IMaterial* m_mFlatMat = nullptr;
	static std::unordered_map<int, bool> m_DrawEnts;
	static bool m_bRunning = false;

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

		for (auto entity : EntityList::m_vecPlayers)
		{
			if (entity == nullptr)
				continue;

			if (!entity->IsPlayer())
				continue;

			if (entity->IsDormant() || !entity->ShouldDraw())
				continue;

			if (!entity->IsAlive())
				continue;
			
			Color color = ESP::GetPlayerColor(entity);
			float flColor[3] = {color.r()/255.0f, color.g()/255.0f, color.b()/255.0f};

			interfaces::RenderView->SetColorModulation(flColor);
			m_bRunning = true;
			entity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);

			int passes = 0;
			auto moveChild = entity->FirstShadowChild();
			while (moveChild != nullptr && passes <= 32)
			{
				if (settings.esp.weapon && static_cast<CBaseEntity*>(moveChild)->IsWeapon())
				{
					color = settings.colors.weapon;
					flColor[0] = color.r()/255.0f;
					flColor[1] = color.g()/255.0f;
					flColor[2] = color.b()/255.0f;
					interfaces::RenderView->SetColorModulation(flColor);
				}

				moveChild->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
				moveChild = moveChild->NextShadowPeer();
				passes++;
			}
			m_bRunning = false;
		}

		interfaces::ModelRender->ForcedMaterialOverride(nullptr);
		interfaces::RenderView->SetColorModulation(savedColor);
		interfaces::RenderView->SetBlend(savedBlend);
	}

	static void Run()
	{
		m_bRunning = false;

		if (!settings.esp.chams)
			return;

		Init();
		DrawPlayers();
	}
}