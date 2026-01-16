#include "entitylist.h"

namespace EntityList
{
	std::vector<CBaseObject*> m_vecBuildings;
	std::vector<CTFPlayer*> m_vecPlayers;
	CTFPlayer* m_pLocalPlayer = nullptr;
	CTFPlayerResource *m_pPlayerResource = nullptr;
	std::vector<CTFPlayer*> m_vecTeammates;
	std::vector<CTFPlayer*> m_vecEnemies;

	void Clear()
	{
		m_vecBuildings.clear();
		m_vecPlayers.clear();
		m_vecTeammates.clear();
		m_vecEnemies.clear();
		m_pLocalPlayer = nullptr;
		m_pPlayerResource = nullptr;
	}

	// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	void Store()
	{
		Clear();

		m_pLocalPlayer = helper::engine::GetLocalPlayer();
		if (m_pLocalPlayer == nullptr)
			return;

		int localTeam = m_pLocalPlayer->m_iTeamNum();

		for (int i = 1; i < interfaces::EntityList->GetHighestEntityIndex(); i++)
		{
			IClientEntity* entity = interfaces::EntityList->GetClientEntity(i);
			if (entity == nullptr)
				continue;

			switch (entity->GetClassID())
			{
				case ETFClassID::CTFPlayer:
				{
					CTFPlayer* player = static_cast<CTFPlayer*>(entity);
					m_vecPlayers.emplace_back(player);

					if (player->m_iTeamNum() == localTeam )
						m_vecTeammates.emplace_back(player);
					else
						m_vecEnemies.emplace_back(player);
					
					break;
				}

				case ETFClassID::CObjectSentrygun:
				case ETFClassID::CObjectDispenser:
				case ETFClassID::CObjectTeleporter:
				{
					m_vecBuildings.emplace_back(static_cast<CBaseObject*>(entity));
					break;
				}

				case ETFClassID::CTFPlayerResource:
				{
					m_pPlayerResource = reinterpret_cast<CTFPlayerResource*>(entity);
					break;
				}

				default: break;
			}
		}
	}

	std::vector<CTFPlayer*> GetPlayers()
	{
		return m_vecPlayers;
	}

	std::vector<CBaseObject*> GetBuildings()
	{
		return m_vecBuildings;
	}

	CTFPlayer* GetLocal()
	{
		return m_pLocalPlayer;
	}

	CTFPlayerResource* GetPlayerResources()
	{
		return m_pPlayerResource;
	}

	std::vector<CTFPlayer*> GetTeammates()
	{
		return m_vecTeammates;
	}

	std::vector<CTFPlayer*> GetEnemies()
	{
		return m_vecEnemies;
	}
}