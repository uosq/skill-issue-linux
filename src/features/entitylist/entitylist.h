#pragma once

#include <vector>
#include "../../sdk/helpers/helper.h"
#include "../../sdk/classes/cbaseobject.h"

namespace EntityList
{
	static std::vector<CBaseObject*> m_vecBuildings;
	static std::vector<CTFPlayer*> m_vecPlayers;
	static CTFPlayer* m_pLocalPlayer = nullptr;

	static void Clear()
	{
		m_vecBuildings.clear();
		m_vecPlayers.clear();
		m_pLocalPlayer = nullptr;
	}

	// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	static void Store()
	{
		Clear();

		m_pLocalPlayer = helper::engine::GetLocalPlayer();

		for (int i = 1; i < interfaces::EntityList->GetHighestEntityIndex(); i++)
		{
			IClientEntity* entity = interfaces::EntityList->GetClientEntity(i);
			if (entity == nullptr)
				continue;

			switch (entity->GetClassID())
			{
				case ETFClassID::CTFPlayer:
				{
					m_vecPlayers.emplace_back(static_cast<CTFPlayer*>(entity));
					break;
				}

				case ETFClassID::CObjectSentrygun:
				case ETFClassID::CObjectDispenser:
				case ETFClassID::CObjectTeleporter:
				{
					m_vecBuildings.emplace_back(static_cast<CBaseObject*>(entity));
					break;
				}

				default: break;
			}
		}
	}

	static std::vector<CTFPlayer*> GetPlayers()
	{
		return m_vecPlayers;
	}

	static std::vector<CBaseObject*> GetBuildings()
	{
		return m_vecBuildings;
	}

	static CTFPlayer* GetLocal()
	{
		return m_pLocalPlayer;
	}
}