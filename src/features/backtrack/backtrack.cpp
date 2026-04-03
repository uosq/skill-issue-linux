#include "backtrack.h"

#include "../../sdk/MaterialManager/materialmanager.h"
#include "../../sdk/helpers/engine/engine.h"
#include "../../sdk/helpers/localplayer/localplayer.h"
#include "../../sdk/interfaces/interfaces.h"

#include "../../settings/settings.h"
#include "../aimbot/utils/utils.h"

// #include "../ticks/ticks.h"

std::unordered_map<int, std::deque<LagCompRecord>> Backtrack::m_records = {};
IMaterial *Backtrack::m_mat						= nullptr;
bool Backtrack::m_drawing						= false;
LagCompRecord *Backtrack::m_current_drawing_record			= nullptr;

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/game/server/player_lagcompensation.cpp#L381-L412
bool LagCompRecord::IsValid()
{
	float correct		 = 0.0f;

	INetChannelInfo *netchan = interfaces::Engine->GetNetChannelInfo();
	if (netchan)
	{
		correct += netchan->GetLatency(FLOW_OUTGOING);
		correct += netchan->GetLatency(FLOW_INCOMING);
	}

	correct += Backtrack::GetInterp();

	static ConVar *sv_maxunlag = interfaces::Cvar->FindVar("sv_maxunlag");
	correct			   = std::clamp(correct, 0.0f, sv_maxunlag->GetFloat());

	float delta		   = correct - (interfaces::GlobalVars->curtime - m_flSimTime);

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

	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc.backtrack);
	if (mode == BacktrackMode::NONE || mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID)
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
			if (!record.IsValid())
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

void Backtrack::CleanRecords()
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
					     [](LagCompRecord &record) { return !record.IsValid(); }),
			      records.end());

		// remove the entry from m_records
		if (records.empty())
			it = m_records.erase(it);
		else
			// or continue
			++it;
	}
}

void Backtrack::Store(CTFPlayer *pLocal, const EntityListEntry &entry)
{
	CleanRecords();

	if (!IsValidPlayer(entry))
		return;

	CTFPlayer *pEntity = static_cast<CTFPlayer *>(entry.ptr);
	if (pEntity == nullptr)
		return;

	if (pLocal->m_iTeamNum() == pEntity->m_iTeamNum())
		return;

	auto &records = m_records[pEntity->GetIndex()];

	if (!records.empty() && records.front().m_flSimTime == pEntity->m_flSimulationTime())
		return;

	matrix3x4 bones[MAXSTUDIOBONES];
	if (!pEntity->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, pEntity->m_flSimulationTime()))
		return;

	records.emplace_front(bones, pEntity->m_flSimulationTime(), pEntity->GetCenter(), pEntity->m_angEyeAngles(),
			      pEntity->EstimateAbsVelocity());
}

void Backtrack::Init()
{
	m_mat = MaterialManager::CreateMaterial("Hello", "UnlitGeneric\n"
							 "{\n"
							 "	$basetexture \"white\"\n"
							 "}");
}

void Backtrack::DoPostScreenSpaceEffects()
{
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

	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc.backtrack);
	if (mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID)
		return;

	float color[] = {1, 1, 1};
	float savedColor[3], savedBlend;

	IMaterial *savedMat;
	OverrideType_t type;

	interfaces::ModelRender->GetMaterialOverride(&savedMat, &type);
	interfaces::RenderView->GetColorModulation(savedColor);
	savedBlend = interfaces::RenderView->GetBlend();

	interfaces::ModelRender->ForcedMaterialOverride(m_mat);
	interfaces::RenderView->SetBlend(0.5f);
	interfaces::RenderView->SetColorModulation(color);

	m_drawing = true;

	switch (mode)
	{
	case BacktrackMode::LAST_ONLY:
	{
		for (auto &[index, records] : m_records)
		{
			CTFPlayer *entity = static_cast<CTFPlayer *>(interfaces::EntityList->GetClientEntity(index));
			if (entity == nullptr)
				continue;

			if (records.empty())
				continue;

			if (!entity->ShouldDraw())
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
			if (entity == nullptr)
				continue;

			if (!entity->ShouldDraw())
				continue;

			if (!entity->IsAlive())
				continue;

			if (records.empty())
				continue;

			if (pLocal->m_iTeamNum() == entity->m_iTeamNum())
				continue;

			for (auto &record : records)
			{
				if (!record.IsValid())
					continue;

				m_current_drawing_record = &record;
				entity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
			}
		}

		break;
	}
	case BacktrackMode::NONE:
	case BacktrackMode::INVALID:
	case BacktrackMode::MAX:
		break;
	}

	m_drawing		 = false;

	m_current_drawing_record = {};
	interfaces::RenderView->SetBlend(savedBlend);
	interfaces::RenderView->SetColorModulation(savedColor);
	interfaces::ModelRender->ForcedMaterialOverride(savedMat);
}

bool Backtrack::GetRecords(CTFPlayer *pEntity, std::vector<LagCompRecord> &out)
{
	auto records = m_records[pEntity->GetIndex()];

	if (records.empty())
		return false;

	// if backtrack is disabled
	// return only the first real record
	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc.backtrack);
	if (mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID || mode == BacktrackMode::NONE)
	{
		LagCompRecord &front = records.front();
		out.emplace_back(front.m_Bones, front.m_flSimTime, front.m_vecAbsCenter, front.m_vecViewAngles,
				 front.m_vecVelocity);
		return true;
	}

	for (const auto &record : records)
		out.emplace_back(record);

	return true;
}

bool Backtrack::IsValidPlayer(const EntityListEntry &entry)
{
	return (entry.flags & (EntityFlags::IsPlayer | EntityFlags::IsEnemy | EntityFlags::IsAlive)) != 0;
}

std::string Backtrack::GetModeName()
{
	switch (static_cast<BacktrackMode>(Settings::Misc.backtrack))
	{
	case BacktrackMode::NONE:
		return "None";
	case BacktrackMode::LAST_ONLY:
		return "Last Record Only";
	case BacktrackMode::ALL_RECORDS:
		return "All Records";
	case BacktrackMode::INVALID:
	case BacktrackMode::MAX:
		break;
	}

	return "Invalid";
}

bool Backtrack::GetReal(CTFPlayer *pEntity, LagCompRecord &out)
{
	if (m_records.empty())
		return false;

	auto records = m_records[pEntity->GetIndex()];
	if (records.empty())
		return false;

	LagCompRecord &front = records.front();
	if (!front.IsValid())
		return false;

	out = front;
	return true;
}