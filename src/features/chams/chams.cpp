#include "chams.h"

namespace Chams
{
	bool m_bMatLoaded = false;
	IMaterial* m_mFlatMat = nullptr;
	bool m_bRunning = false;
	std::unordered_set<int> m_Entities;

	bool Init()
	{
		if (m_mFlatMat == nullptr)
		{
			m_mFlatMat = MaterialManager::CreateMaterial("SKFlatMat",
			"UnlitGeneric\n"
				"{\n"
				"	$basetexture \"white\"\n"
				"}"
			);

			m_bMatLoaded = true;

			#ifdef DEBUG
			helper::console::Print("Material vapooo created!\n");
			#endif
		}

		return m_bMatLoaded;
	}

	bool ShouldHide(int entindex)
	{
		return m_Entities.find(entindex) != m_Entities.end();
	}

	void DrawPlayers()
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
			
			Color color = ESP_Utils::GetEntityColor(entity);
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
				if (Settings::ESP::weapon && attachment->IsWeapon())
				{
					color = Settings::Colors::weapon;
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

	void Run()
	{
		if (!m_Entities.empty())
			m_Entities.clear();

		m_bRunning = false;

		if (!Settings::ESP::chams)
			return;

		if (interfaces::Engine->IsTakingScreenshot())
			return;

		DrawPlayers();
	}
}