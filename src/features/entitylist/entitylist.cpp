#include "entitylist.h"
#include "structs.h"

#include "../../gui/utils/string_utils.h"

void EntityList::Clear()
{
	s_vecEntities.clear();
	s_pLocalPlayer = nullptr;
	s_pPlayerResource = nullptr;
	s_vecStaticEntities.clear();
}

void EntityList::StoreBaseAnimating(CBaseAnimating* pAnimating)
{
	if (pAnimating == nullptr)
		return;

	const model_t* model = pAnimating->GetModel();

	if (model == nullptr)
		return;

	const char* model_name = interfaces::ModelInfoClient->GetModelName(model);

	if (model_name == nullptr || model_name[0] == '\0')
		return;

	// yes, maybe using fnv's hashing might be more efficient here
	// but i like it readable and i'm too lazy to check what is the exact model name

	if (ContainsInsensitive(model_name, "ammopack"))
		s_vecStaticEntities.emplace_back(StaticEntity{StaticEntityType::AMMOPACK, pAnimating});
	else if (ContainsInsensitive(model_name, "medkit"))
		s_vecStaticEntities.emplace_back(StaticEntity{StaticEntityType::MEDKIT, pAnimating});

	//Logs::Info(std::string("Found CBaseAnimating with model ") + model_name);
}

// Call in LevelInitPostEntity
void EntityList::Reserve()
{
	s_vecEntities.reserve(128);
	s_vecStaticEntities.reserve(128);

	// interfaces::Cvar->ConsolePrintf("Reserved %i entities\n",
	// maxentities);
}

// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
void EntityList::Store()
{
	Clear();

	s_pLocalPlayer = helper::engine::GetLocalPlayer();
	if (s_pLocalPlayer == nullptr)
		return;

	int localTeam = s_pLocalPlayer->m_iTeamNum();

	for (int i = 1; i <= interfaces::EntityList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *entity = interfaces::EntityList->GetClientEntity(i);
		if (entity == nullptr || entity->IsDormant())
			continue;

		switch (entity->GetClassID())
		{
		case ETFClassID::CTFPlayer:
		{
			CTFPlayer *player = static_cast<CTFPlayer *>(entity);

			// cant skip lp or else ESP, Chams and Glow
			// don't highlight us skip localplayer
			// if (player == m_pLocalPlayer)
			// break;

			EntityListEntry entry;
			entry.flags = EntityFlags::IsPlayer;
			entry.ptr   = player;

			if (player->m_iTeamNum() != localTeam)
				entry.flags |= EntityFlags::IsEnemy;

			if (player->IsAlive())
				entry.flags |= EntityFlags::IsAlive;

			s_vecEntities.emplace_back(entry);

			// skip localplayer :))
			// btw this is fucking bullshit
			// calling Backtrack::Store() directly on
			// FrameStageNotify does not work
			//if (player->GetIndex() != m_pLocalPlayer->GetIndex())
				//Backtrack::Store(m_pLocalPlayer, entry);

			break;
		}

		case ETFClassID::CObjectSentrygun:
		case ETFClassID::CObjectDispenser:
		case ETFClassID::CObjectTeleporter:
		{
			CBaseObject *building = static_cast<CBaseObject *>(entity);
			if (building->m_iHealth() <= 0)
				break;

			EntityListEntry entry = {};
			entry.flags	      = EntityFlags::IsBuilding | EntityFlags::IsAlive;
			entry.ptr	      = building;

			if (building->m_iTeamNum() != localTeam)
				entry.flags |= EntityFlags::IsEnemy;

			s_vecEntities.emplace_back(entry);
			break;
		}

		case ETFClassID::CTFPlayerResource:
		{
			s_pPlayerResource = static_cast<CTFPlayerResource *>(entity);
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
			CBaseProjectile *baseEnt = static_cast<CBaseProjectile *>(entity);

			EntityListEntry entry	 = {};
			entry.flags		 = EntityFlags::IsProjectile | EntityFlags::IsAlive;
			entry.ptr		 = baseEnt;

			if (baseEnt->m_iTeamNum() != localTeam)
				entry.flags |= EntityFlags::IsEnemy;

			s_vecEntities.emplace_back(entry);
			break;
		}

		case ETFClassID::CBaseAnimating:
		{
			CBaseAnimating* pAnimating = static_cast<CBaseAnimating*>(entity);
			StoreBaseAnimating(pAnimating);
			break;
		}

		default:
			break;
		}
	}
}

CTFPlayer* EntityList::GetLocal()
{
	return s_pLocalPlayer;
}

CTFPlayerResource* EntityList::GetPlayerResources()
{
	return s_pPlayerResource;
}

const std::vector<EntityListEntry>& EntityList::GetEntities()
{
	return s_vecEntities;
}

std::vector<EntityListEntry> EntityList::GetTeammates()
{
	std::vector<EntityListEntry> teammates;

	for (const auto &entry : s_vecEntities)
	{
		if (entry.flags & EntityFlags::IsEnemy)
			continue;

		teammates.emplace_back(entry);
	}

	return teammates;
}

std::vector<EntityListEntry> EntityList::GetEnemies()
{
	std::vector<EntityListEntry> enemies;

	for (const auto &entry : s_vecEntities)
	{
		if (!(entry.flags & EntityFlags::IsEnemy))
			continue;

		enemies.emplace_back(entry);
	}

	return enemies;
}

CBaseEntity* EntityList::GetAimbotTarget()
{
	return s_pAimbotTarget;
}

void EntityList::SetAimbotTarget(CBaseEntity* pEntity)
{
	s_pAimbotTarget = pEntity;
}

const std::vector<StaticEntity>& EntityList::GetStaticEntities()
{
	return s_vecStaticEntities;
}