#pragma once

#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/helpers/helper.h"
#include "../../features/esp/esp.h"
#include "../entitylist/entitylist.h"
#include <unordered_set>
#include <vector>

namespace Chams
{
	static bool m_bMatLoaded = false;
	static IMaterial* m_mFlatMat = nullptr;
	static bool m_bRunning = false;
	static std::unordered_set<int> m_Entities;

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
		return m_Entities.find(entindex) != m_Entities.end();
	}

	static void DrawPlayers()
	{
		float savedColor[3], savedBlend;
		savedBlend = interfaces::RenderView->GetBlend();

		interfaces::RenderView->GetColorModulation(savedColor);
		interfaces::RenderView->SetBlend(0.5f);
		interfaces::ModelRender->ForcedMaterialOverride(m_mFlatMat);

		for (auto& entry : EntityList::GetEntities())
		{
			CTFPlayer* entity = static_cast<CTFPlayer*>(entry.ptr);
			if (entity == nullptr)
				continue;

			if (!(entry.flags & EntityFlags::IsPlayer))
				continue;

			if (!entity->ShouldDraw())
				continue;

			if (!entity->IsAlive())
				continue;
			
			Color color = ESP::GetPlayerColor(entity);
			float flColor[3] = {color.r()/255.0f, color.g()/255.0f, color.b()/255.0f};

			interfaces::RenderView->SetColorModulation(flColor);
			m_bRunning = true;
			m_Entities.insert(entity->GetIndex());
			entity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);

			int passes = 0;
			auto moveChild = entity->FirstShadowChild();
			while (moveChild != nullptr && passes <= 32)
			{
				CBaseEntity* attachment = static_cast<CBaseEntity*>(moveChild);
				if (g_Settings.esp.weapon && attachment->IsWeapon())
				{
					color = g_Settings.colors.weapon;
					flColor[0] = color.r()/255.0f;
					flColor[1] = color.g()/255.0f;
					flColor[2] = color.b()/255.0f;
					interfaces::RenderView->SetColorModulation(flColor);
				}

				m_Entities.insert(attachment->GetIndex());
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
		if (!m_Entities.empty())
			m_Entities.clear();

		m_bRunning = false;

		if (!g_Settings.esp.chams)
			return;

		if (interfaces::Engine->IsTakingScreenshot())
			return;

		Init();
		DrawPlayers();
	}
}