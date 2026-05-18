#include "backtrack.h"

#include "../../sdk/helpers/engine/engine.h"
#include "../../sdk/helpers/localplayer/localplayer.h"
#include "../../sdk/interfaces/interfaces.h"

#include "../../settings/settings.h"
#include "../aimbot/utils/utils.h"

#include "../materialregistry/reg.h"

// #include "../ticks/ticks.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/server/player_lagcompensation.cpp#L381-L412
bool LagCompRecord::IsValid(CUserCmd* pCmd)
{
	float correct		 = 0.0f;

	INetChannelInfo *netchan = interfaces::Engine->GetNetChannelInfo();
	if (netchan)
	{
		correct += netchan->GetLatency(FLOW_OUTGOING);
		correct += netchan->GetLatency(FLOW_INCOMING);
	}

	correct += features::backtrack.GetInterp();

	static ConVar *sv_maxunlag = interfaces::Cvar->FindVar("sv_maxunlag");
	correct			   = std::clamp(correct, 0.0f, sv_maxunlag->GetFloat());

	float delta		   = correct - (TICKS_TO_TIME(pCmd->tick_count) - m_flSimTime);

	return std::fabs(delta) < 0.2f;
}

float Backtrack::GetLatency()
{
	float flLatency = 0.0f;

	if (auto netchan = interfaces::Engine->GetNetChannelInfo())
		flLatency += netchan->GetLatency(FLOW_INCOMING) + netchan->GetLatency(FLOW_OUTGOING);

	return flLatency;
}

bool ShouldPrioritizeHead(CTFWeaponBase *pWeapon)
{
	if (pWeapon->IsAmbassador() && pWeapon->CanAmbassadorHeadshot())
		return true;

	if (pWeapon->IsSniperRifle() && static_cast<CTFSniperRifle *>(pWeapon)->m_flChargedDamage() > 50.0f)
		return true;

	return false;
}

void Backtrack::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	if (pWeapon->GetWeaponType() == EWeaponType::PROJECTILE || pWeapon->GetWeaponType() == EWeaponType::UNKNOWN)
		return;

	if (!Config.backtrack.packed.enabled)
		return;

	if (!helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		return;

	Vector &viewAngles	   = pCmd->viewangles;
	Vector eyePos		   = pLocal->GetEyePos();

	LagCompRecord *pBestRecord = nullptr;
	float flBestFov		   = AimbotUtils::GetFovScaled(30.0f);

	bool bPriotizeHead	   = ShouldPrioritizeHead(pWeapon);

	for (auto &[index, records] : m_records)
	{
		CTFPlayer *entity = static_cast<CTFPlayer *>(interfaces::EntityList->GetClientEntity(index));
		if (entity == nullptr)
			continue;

		if (pLocal->GetIndex() == entity->GetIndex())
			continue;

		if (entity->m_iTeamNum() == pLocal->m_iTeamNum())
			continue;

		CBaseAnimating *pAnimating = reinterpret_cast<CBaseAnimating *>(entity);
		if (pAnimating == nullptr)
			continue;

		for (auto &record : records)
		{
			if (!record.IsValid(pCmd))
				continue;

			Vector aimPos;
			if (!pAnimating->GetHitboxCenter(record.m_Bones, bPriotizeHead ? HITBOX_HEAD : HITBOX_PELVIS,
							 aimPos))
				continue;

			Vector angle;
			Math::VectorAngles(aimPos - eyePos, angle);

			float flFov = Math::CalcFov(viewAngles, angle);
			if (flFov < flBestFov)
			{
				pBestRecord = &record;
				flBestFov   = flFov;
			}
		}
	}

	if (pBestRecord == nullptr)
		return;

	pCmd->tick_count = TIME_TO_TICKS(pBestRecord->m_flSimTime + GetInterp());
}

void Backtrack::Reset()
{
	m_records.clear();
}

float Backtrack::GetInterp()
{
	static ConVar *cl_interp       = interfaces::Cvar->FindVar("cl_interp");
	static ConVar *cl_updaterate   = interfaces::Cvar->FindVar("cl_updaterate");
	static ConVar *cl_interp_ratio = interfaces::Cvar->FindVar("cl_interp_ratio");

	float lerp = std::max(cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat());

	return lerp;
}

void Backtrack::CleanRecords(CUserCmd* pCmd)
{
	// i wrote this code at 2 am
	// i will probably not remember
	// how it works after tomorow
	// todo: make this shit look good
	for (auto it = m_records.begin(); it != m_records.end();)
	{
		CTFPlayer *entity = static_cast<CTFPlayer *>(interfaces::EntityList->GetClientEntity(it->first));
		auto &records	  = it->second;

		if (!entity || !entity->IsAlive())
		{
			it = m_records.erase(it);
			continue;
		}

		records.erase(std::remove_if(records.begin(), records.end(),
					     [&](LagCompRecord &record) { return !record.IsValid(pCmd); }),
			      records.end());

		// remove the entry from m_records
		if (records.empty())
			it = m_records.erase(it);
		else
			// or continue
			++it;
	}
}

void Backtrack::Store()
{
	if (!Config.backtrack.packed.enabled)
		return;

	CTFPlayer* pLocal = features::entities.GetLocal();
	if (pLocal == nullptr)
		return;

	int localTeam = pLocal->m_iTeamNum();
	auto enemies = features::entities.GetEnemies();

	for (const auto& entry : enemies)
	{
		if (!IsValidPlayer(entry))
			continue;

		if (entry.ptr->IsBuilding())
			continue;

		CTFPlayer* pEntity = static_cast<CTFPlayer*>(entry.ptr);

		if (localTeam == pEntity->m_iTeamNum())
			continue;

		auto &records = m_records[pEntity->GetIndex()];

		if (!records.empty() && records.front().m_flSimTime == pEntity->m_flSimulationTime())
			continue;

		matrix3x4 bones[MAXSTUDIOBONES];
		if (!pEntity->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, pEntity->m_flSimulationTime()))
			continue;

		records.emplace_front
		(
			bones,
			pEntity->m_flSimulationTime(),
			pEntity->GetCenter(),
			pEntity->m_angEyeAngles(),
			pEntity->EstimateAbsVelocity(),
			pEntity->m_vecMins(), pEntity->m_vecMaxs(),
			pEntity->m_vecOrigin()
		);
	}
}

void Backtrack::Init()
{
	Config.backtrack.active_materials.emplace_back("basic flat");
	Config.backtrack.active_materials.emplace_back("basic shaded");
}

void Backtrack::DoPostScreenSpaceEffects()
{
	if (!Config.backtrack.packed.enabled)
		return;

	if (m_records.empty())
		return;

	CTFPlayer *pLocal = helper::engine::GetLocalPlayer();
	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	CTFWeaponBase *pWeapon = HandleAs<CTFWeaponBase *>(pLocal->GetActiveWeapon());
	if (pWeapon == nullptr)
		return;

	// only hitscan and melee!
	if (!(pWeapon->IsMelee() || pWeapon->IsHitscan()))
		return;

	BacktrackMode mode = static_cast<BacktrackMode>(Config.backtrack.packed.draw_mode);
	if (mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID || mode == BacktrackMode::NONE)
		return;

	// no materials no drawing
	if (Config.backtrack.active_materials.empty())
		return;

	float savedColor[3], savedBlend;
	IMaterial *savedMat;
	OverrideType_t type;

	// save our cool stuff
	interfaces::ModelRender->GetMaterialOverride(&savedMat, &type);
	interfaces::RenderView->GetColorModulation(savedColor);
	savedBlend = interfaces::RenderView->GetBlend();

	float color[] = {1.0f, 1.0f, 1.0f};
	interfaces::RenderView->SetColorModulation(color);

	auto DrawRecords = [&]()
	{
		switch (mode)
		{
		case BacktrackMode::LAST_ONLY:
		{
			for (auto &[index, records] : m_records)
			{
				if (records.empty())
					continue;

				CTFPlayer *entity = static_cast<CTFPlayer *>(interfaces::EntityList->GetClientEntity(index));
				if (entity == nullptr || !entity->ShouldDraw())
					continue;

				if (pLocal->m_iTeamNum() == entity->m_iTeamNum())
					continue;

				m_current_drawing_record = &records.back();
				entity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
			}
			break;
		}
		case BacktrackMode::ALL_RECORDS:
		{
			for (auto &[index, records] : m_records)
			{
				CTFPlayer *entity = static_cast<CTFPlayer *>(interfaces::EntityList->GetClientEntity(index));
				if (entity == nullptr || !entity->ShouldDraw() || !entity->IsAlive())
					continue;

				if (records.empty() || pLocal->m_iTeamNum() == entity->m_iTeamNum())
					continue;

				for (auto &record : records)
				{
					m_current_drawing_record = &record;
					entity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
				}
			}
			break;
		}
		default:
			break;
		}
	};

	for (const auto& mat_name : Config.backtrack.active_materials)
	{
		auto mat = features::material_registry.GetMaterialByName(mat_name);

		if (!mat || !mat->IsValidMat())
			continue;

		interfaces::RenderView->SetBlend(mat->GetAlpha());
		interfaces::ModelRender->ForcedMaterialOverride(mat->GetMaterial());

		m_drawing = true;
		DrawRecords();
		m_drawing = false;
	}

	m_current_drawing_record = nullptr;

	interfaces::RenderView->SetBlend(savedBlend);
	interfaces::RenderView->SetColorModulation(savedColor);
	interfaces::ModelRender->ForcedMaterialOverride(savedMat);
}

bool Backtrack::GetRecords(CTFPlayer *pEntity, std::vector<LagCompRecord> &out)
{
	if (!Config.backtrack.packed.enabled)
	{
		matrix3x4 bones[MAXSTUDIOBONES];
		if (!pEntity->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, interfaces::GlobalVars->curtime))
			return false;

		out.emplace_back
		(
			bones,
			pEntity->m_flSimulationTime(),
			pEntity->GetCenter(),
			pEntity->GetAbsAngles(),
			pEntity->EstimateAbsVelocity(),
			pEntity->m_vecMins(), pEntity->m_vecMaxs(),
			pEntity->m_vecOrigin()
		);

		return true;
	}

	auto it = m_records.find(pEntity->GetIndex());
	if (it == m_records.end() || it->second.empty())
		return false;

	auto& records = it->second;

	for (const auto &record : records)
		out.emplace_back(record);

	return true;
}

bool Backtrack::IsValidPlayer(const EntityListEntry &entry)
{
	return entry.ptr != nullptr && (entry.flags & (EntityFlags::IsPlayer | EntityFlags::IsEnemy | EntityFlags::IsAlive));
}

bool Backtrack::GetReal(CTFPlayer *pEntity, LagCompRecord &out)
{
	if (m_records.empty())
		return false;

	auto it = m_records.find(pEntity->GetIndex());
	if (it == m_records.end() || it->second.empty())
		return false;

	out = it->second.front();
	return true;
}

bool Backtrack::IsDrawing()
{
	return m_drawing;
}

std::optional<LagCompRecord> Backtrack::GetDrawingRecord()
{
	return m_drawing ? *m_current_drawing_record : LagCompRecord{};
}