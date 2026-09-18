#include "autodetonate.h"

#include "../../entitylist/entitylist.h"
#include "../../logs/logs.h"

#include "../../../sdk/classes/projectiles.h"
#include "../../../sdk/classes/weaponbase.h"

#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/definitions/cgametrace.h"

constexpr int MAX_STICKIES = 14; // Scottish Resistance

using sticky_list = std::array<CTFGrenadePipebombProjectile*, MAX_STICKIES>;
using entity_list = std::vector<EntityListEntry>;

static sticky_list get_stickies(const entity_list& entities, CTFWeaponBase* weapon)
{
	sticky_list projectiles {nullptr};

	int count = 0;
	const int weapon_index = weapon->entindex();

	for (auto& entity : entities)
	{
		if (!entity.ptr)
			continue;

		if (!(entity.flags & EntityFlags::IsProjectile))
			continue;

		if (entity.ptr->GetClassID() != ETFClassID::CTFGrenadePipebombProjectile)
			continue;

		if (entity.flags & EntityFlags::IsEnemy)
			continue;

		auto* proj = reinterpret_cast<CTFGrenadePipebombProjectile*>(entity.ptr);
		auto launcher = proj->m_hLauncher().Get();

		if (!launcher || launcher->GetIndex() != weapon_index)
			continue;

		projectiles[count] = proj;
		count++;
	}

	return projectiles;
}

static bool is_valid_target(CBaseEntity* entity, CTFPlayer* local)
{
	return entity && !entity->IsDormant() && (entity->IsPlayer() || entity->IsBuilding()) && (entity->m_iTeamNum() != local->m_iTeamNum());
}

static void run_stickybomb_launcher(CUserCmd* cmd, CTFPlayer* local, CTFWeaponBase* weapon, const int local_team, const sticky_list& stickies)
{
	constexpr int SCOTTISH_RESISTANCE_INDEX = 130;

	const int& def_index = weapon->m_iItemDefinitionIndex();
	const bool is_scottish = def_index == SCOTTISH_RESISTANCE_INDEX;

	for (const auto& sticky : stickies)
	{
		if (!sticky || !sticky->CanExplode())
			return; // everything after this is nullptr

		CBaseEntity* list[64] = {nullptr};
		CFlaggedEntitiesEnum entities {list, 64, 0};

		const Vec3& origin = sticky->GetAbsOrigin();
		const float damage_radius = sticky->GetDamageRadius();
		
		// let the engine cook
		interfaces::partition->EnumerateElementsInSphere(PARTITION_CLIENT_NON_STATIC_EDICTS, origin, damage_radius, false, &entities);

		const int count = entities.GetCount();

		if (count <= 0)
			continue;

		for (int i = 0; i < count; i++)
		{
			CBaseEntity* entity = list[i];

			if (!is_valid_target(entity, local))
				continue;

			if (entity == local)
				continue;

			CGameTrace trace;
			CTraceFilterHitscan filter;
			filter.pSkip = sticky;

			helper::engine::Trace(origin, entity->GetAbsOrigin(), MASK_SHOT_HULL, &filter, &trace);

			if (!trace.DidHit() || !trace.m_pEnt || trace.m_pEnt->GetIndex() != entity->GetIndex())
				continue;

			cmd->buttons |= IN_ATTACK2;

			if (is_scottish)
				cmd->viewangles = Math::CalcAngle(local->GetEyePos(), sticky->GetCenter());

			return;
		}
	}
}

void AutoDetonate::Run(CUserCmd* cmd, CTFPlayer* local)
{
	if (!local) return;

	CTFPipebombLauncher* pipebomb = reinterpret_cast<CTFPipebombLauncher*>(local->GetEntityFromLoadoutSlot(LOADOUT_POSITION_SECONDARY));

	if (!pipebomb || pipebomb->m_iPipebombCount() <= 0)
		return;

	const auto& entities = features::entities.GetEntities();
	auto stickies = get_stickies(entities, pipebomb);

	// wtf
	if (stickies.empty())
	{
		features::logs.Error("Couldn't find stickies");
		return;
	}

	const int local_team = local->m_iTeamNum();

	run_stickybomb_launcher(cmd, local, pipebomb, local_team, stickies);
}
