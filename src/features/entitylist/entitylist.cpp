#include "entitylist.h"

namespace EntityList
{
	CTFPlayer* m_pLocalPlayer = nullptr;
	CTFPlayerResource *m_pPlayerResource = nullptr;
	CBaseEntity* m_pAimbotTarget = nullptr;
	std::vector<EntityListEntry> m_vecEntities;

	void Clear()
	{
		m_vecEntities.clear();
		m_pLocalPlayer = nullptr;
		m_pPlayerResource = nullptr;
	}

	// Call in LevelInitPostEntity
	void Reserve()
	{
		int maxentities = interfaces::EntityList->GetMaxEntities();
		m_vecEntities.reserve(maxentities);

		//interfaces::Cvar->ConsolePrintf("Reserved %i entities\n", maxentities);
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
			if (entity == nullptr || entity->IsDormant())
				continue;

			switch (entity->GetClassID())
			{
				case ETFClassID::CTFPlayer:
				{
					CTFPlayer* player = static_cast<CTFPlayer*>(entity);
					if (player == nullptr)
						break;

					EntityListEntry entry;
					entry.flags = EntityFlags::IsPlayer;
					entry.ptr = player;
					
					if (player->m_iTeamNum() != localTeam)
						entry.flags |= EntityFlags::IsEnemy;
			
					if (player->IsAlive())
						entry.flags |= EntityFlags::IsAlive;

					m_vecEntities.emplace_back(entry);
					break;
				}

				case ETFClassID::CObjectSentrygun:
				case ETFClassID::CObjectDispenser:
				case ETFClassID::CObjectTeleporter:
				{
					CBaseObject* building = static_cast<CBaseObject*>(entity);
					if (building == nullptr || building->m_iHealth() <= 0)
						break;

					EntityListEntry entry = {};
					entry.flags = EntityFlags::IsBuilding | EntityFlags::IsAlive;
					entry.ptr = building;

					if (building->m_iTeamNum() != localTeam)
						entry.flags |= EntityFlags::IsEnemy;

					m_vecEntities.emplace_back(entry);
					break;
				}

				case ETFClassID::CTFPlayerResource:
				{
					m_pPlayerResource = static_cast<CTFPlayerResource*>(entity);
					break;
				}

				case ETFClassID::CTFProjectile_Rocket:
				case ETFClassID::CTFProjectile_Arrow:
				case ETFClassID::CTFProjectile_BallOfFire:
				case ETFClassID::CTFProjectile_Flare:
				case ETFClassID::CTFProjectile_HealingBolt:
				case ETFClassID::CTFProjectile_Jar:
				case ETFClassID::CTFProjectile_JarGas:
				case ETFClassID::CTFProjectile_JarMilk:
				case ETFClassID::CTFProjectile_SentryRocket:
				case ETFClassID::CTFStickBomb:
				{
					CBaseProjectile* baseEnt = static_cast<CBaseProjectile*>(entity);
					if (baseEnt == nullptr)
						break;

					EntityListEntry entry = {};
					entry.flags = EntityFlags::IsProjectile | EntityFlags::IsAlive;
					entry.ptr = baseEnt;

					if (baseEnt->m_iTeamNum() != localTeam)
						entry.flags |= EntityFlags::IsEnemy;

					m_vecEntities.emplace_back(entry);
					break;
				}

				default: break;
			}
		}
	}

	CTFPlayer* GetLocal()
	{
		return m_pLocalPlayer;
	}

	CTFPlayerResource* GetPlayerResources()
	{
		return m_pPlayerResource;
	}

	const std::vector<EntityListEntry>& GetEntities()
	{
		return m_vecEntities;
	}

	std::vector<EntityListEntry> GetTeammates()
	{
		std::vector<EntityListEntry> teammates;

		for (const auto& entry : m_vecEntities)
		{
			if (entry.flags & EntityFlags::IsEnemy)
				continue;

			teammates.emplace_back(entry);
		}

		return teammates;
	}

	std::vector<EntityListEntry> GetEnemies()
	{
		std::vector<EntityListEntry> enemies;

		for (const auto& entry : m_vecEntities)
		{
			if (!(entry.flags & EntityFlags::IsEnemy))
				continue;

			enemies.emplace_back(entry);
		}

		return enemies;
	}
}