#include "backtrack.h"

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/MaterialManager/materialmanager.h"
#include "../../sdk/helpers/localplayer/localplayer.h"

#include "../../settings/settings.h"

//#include "../ticks/ticks.h"

std::unordered_map<int, std::deque<LagCompRecord>> Backtrack::m_records = {};
IMaterial* Backtrack::m_mat = nullptr;
bool Backtrack::m_drawing = false;
LagCompRecord* Backtrack::m_current_drawing_record = nullptr;

static const int sniperPriorityHitboxes[]
{
	HITBOX_HEAD,
	HITBOX_SPINE0,
	HITBOX_SPINE3,
	HITBOX_PELVIS,
};

static const int normalPriorityHitboxes[]
{
	HITBOX_SPINE0,
	HITBOX_SPINE3,
	HITBOX_PELVIS,
	HITBOX_HEAD,
};

//bool LagCompRecord::IsValid(CUserCmd* pCmd)
bool LagCompRecord::IsValid()
{
	/*float correct = 0.0f;
	Have to think on how to pass pCmd's tick_count on FSN

	INetChannelInfo* netchan = interfaces::Engine->GetNetChannelInfo();

	// add network latency
	if (netchan != nullptr)
		correct += netchan->GetLatency(FLOW_OUTGOING);

	static ConVar* cl_interp = interfaces::Cvar->FindVar("cl_interp");
	static ConVar* cl_updaterate = interfaces::Cvar->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = interfaces::Cvar->FindVar("cl_interp_ratio");

	float lerp_time = std::max(cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat());
	correct += lerp_time;

	static ConVar* sv_maxunlag = interfaces::Cvar->FindVar("sv_maxunlag");
	correct = std::clamp(correct, 0.0f, sv_maxunlag->GetFloat());

	int targettick = pCmd->tick_count - TIME_TO_TICKS(lerp_time);

	float delta_tick = correct - TICKS_TO_TIME(interfaces::GlobalVars->tickcount - targettick);

	return std::fabs(delta_tick) <= 0.2f;*/

	float time = interfaces::GlobalVars->curtime;

	INetChannelInfo* netchan = interfaces::Engine->GetNetChannelInfo();

	if (netchan)
		time += netchan->GetLatency(FLOW_OUTGOING);

	return (time - simtime) <= 0.2f;
}

bool ShouldPrioritizeHead(CTFWeaponBase* pWeapon)
{
	if (pWeapon->IsAmbassador() && pWeapon->CanAmbassadorHeadshot())
		return true;

	if (pWeapon->IsSniperRifle() && static_cast<CTFSniperRifle*>(pWeapon)->m_flChargedDamage() > 50.0f)
		return true;

	return false;
}

void Backtrack::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd *pCmd)
{
	if (pWeapon->GetWeaponType() == EWeaponType::PROJECTILE
	|| pWeapon->GetWeaponType() == EWeaponType::UNKNOWN)
		return;

	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc.backtrack);
	if (mode == BacktrackMode::NONE || mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID)
		return;

	if (!helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		return;

	Vector& viewAngles = pCmd->viewangles;
	Vector eyePos = pLocal->GetEyePos();

	LagCompRecord* pBestHeadRecord = nullptr;
	float flBestHeadFov = 30.0f;

	LagCompRecord* pBestBodyRecord = nullptr;
	float flBestBodyFov = 30.0f;

	const auto& hitboxes = ShouldPrioritizeHead(pWeapon) ? sniperPriorityHitboxes : normalPriorityHitboxes;

	for (auto& [index, records] : m_records)
	{
		CTFPlayer* entity = static_cast<CTFPlayer*>(interfaces::EntityList->GetClientEntity(index));
		if (entity == nullptr)
			continue;

		CBaseAnimating* pAnimating = reinterpret_cast<CBaseAnimating*>(entity);
		if (pAnimating == nullptr)
			continue;

		for (auto& record : records)
		{
			if (!record.IsValid())
				continue;

			for (const auto& hitbox : hitboxes)
			{
				Vector aimPos;
				if (!pAnimating->GetHitboxCenter(record.bones, hitbox, aimPos))
					continue;

				Vector angle;
				Math::VectorAngles(aimPos - eyePos, angle);

				float flFov = Math::CalcFov(viewAngles, angle);
				if (hitbox == HITBOX_HEAD)
				{
					if (flFov < flBestHeadFov)
					{
						flBestHeadFov = flFov;
						pBestHeadRecord = &record;
					}
				}
				else
				{
					if (flFov < flBestBodyFov)
					{
						flBestBodyFov = flFov;
						pBestBodyRecord = &record;
					}
				}
			}
		}
	}

	LagCompRecord* bestRecord = pBestHeadRecord ? pBestHeadRecord : pBestBodyRecord;
	if (bestRecord == nullptr)
		return;

	float flLatency = 0.0f;

	if (auto* netchan = interfaces::Engine->GetNetChannelInfo(); netchan != nullptr)
		flLatency = netchan->GetLatency(FLOW_OUTGOING);

	int tick = TIME_TO_TICKS(bestRecord->simtime + flLatency);
	pCmd->tick_count = tick;
}

void Backtrack::Reset()
{
	m_records.clear();
}

void Backtrack::CleanRecords()
{
	// i wrote this code at 2 am
	// i will probably not remember
	// how it works after tomorow
	// todo: make this shit look good
	for (auto it = m_records.begin(); it != m_records.end();)
	{
		CTFPlayer* entity = static_cast<CTFPlayer*>(interfaces::EntityList->GetClientEntity(it->first));
		auto& records = it->second;

		if (!entity || !entity->IsAlive())
		{
			it = m_records.erase(it);
			continue;
		}

		records.erase
		(
			std::remove_if(records.begin(), records.end(),
				[](LagCompRecord& record) { return !record.IsValid(); }),
			records.end()
		);

		// remove the entry from m_records
		if (records.empty())
			it = m_records.erase(it);
		else
		// or continue
			++it;
	}
}

void Backtrack::Store(const EntityListEntry& entry)
{
	CleanRecords();

	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc.backtrack);
	if (mode == BacktrackMode::NONE || mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID)
		return;

	if (!IsValidPlayer(entry))
		return;

	CTFPlayer* pEntity = static_cast<CTFPlayer*>(entry.ptr);
	if (pEntity == nullptr)
		return;

	auto& records = m_records[pEntity->GetIndex()];

	if (!records.empty() && records.front().simtime == pEntity->m_flSimulationTime())
			return;

	matrix3x4 bones[MAXSTUDIOBONES];
	if (!pEntity->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, pEntity->m_flSimulationTime()))
		return;

	LagCompRecord record
	{
		bones,
		pEntity->m_flSimulationTime(),
		pEntity->GetCenter(),
		pEntity->m_angEyeAngles()
	};

	records.push_front(record);
}

void Backtrack::Init()
{
	m_mat = MaterialManager::CreateMaterial("Hello", 
		"UnlitGeneric\n"
		"{\n"
		"	$basetexture \"white\"\n"
		"}"
	);
}

void Backtrack::DoPostScreenSpaceEffects()
{
	if (m_records.empty())
		return;

	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc.backtrack);
	if (mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID)
		return;

        float color[] = {1, 1, 1};
	float savedColor[3], savedBlend;

	IMaterial* savedMat;
	OverrideType_t type;

	interfaces::ModelRender->GetMaterialOverride(&savedMat, &type);
	interfaces::RenderView->GetColorModulation(savedColor);
	savedBlend = interfaces::RenderView->GetBlend();

	interfaces::ModelRender->ForcedMaterialOverride(m_mat);
	interfaces::RenderView->SetBlend(0.5f);
	interfaces::RenderView->SetColorModulation(color);

	m_drawing = true;

	switch(mode)
	{
        case BacktrackMode::LAST_ONLY:
	{
		for (auto& [index, records] : m_records)
		{
			CTFPlayer* entity = static_cast<CTFPlayer*>(interfaces::EntityList->GetClientEntity(index));
			if (entity == nullptr)
				continue;

			if (records.empty())
				continue;

			if (!entity->ShouldDraw())
				continue;

			m_current_drawing_record = &records.back();
			entity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
		}

		break;
	}
        case BacktrackMode::ALL_RECORDS:
	{
		for (auto& [index, records] : m_records)
		{
			CTFPlayer* entity = static_cast<CTFPlayer*>(interfaces::EntityList->GetClientEntity(index));
			if (entity == nullptr)
				continue;

			if (!entity->ShouldDraw())
				continue;

			if (!entity->IsAlive())
				continue;

			if (records.empty())
				continue;

			for (auto& record : records)
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

	m_drawing = false;

	m_current_drawing_record = {};
	interfaces::RenderView->SetBlend(savedBlend);
	interfaces::RenderView->SetColorModulation(savedColor);
	interfaces::ModelRender->ForcedMaterialOverride(savedMat);
}

bool Backtrack::GetRecords(CTFPlayer* pEntity, std::vector<LagCompRecord>& out)
{
	/*BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc.backtrack);
	if (mode >= BacktrackMode::MAX && mode <= BacktrackMode::INVALID)
	{
		LagCompRecord origin = {};
		if (!pEntity->SetupBones(origin.bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, interfaces::GlobalVars->curtime))
			return false;

		origin.absCenter = pEntity->GetCenter();
		origin.simtime = pEntity->m_flSimulationTime();
		origin.viewAngle = pEntity->m_angEyeAngles();

		return true;
	}*/

	auto records = m_records[pEntity->GetIndex()];

	if (records.empty())
		return false;

	for (const auto& record : records)
		out.push_back(record);

	return true;
}

bool Backtrack::IsValidPlayer(const EntityListEntry& entry)
{
	return (entry.flags & (EntityFlags::IsPlayer | EntityFlags::IsEnemy | EntityFlags::IsAlive)) != 0;
}

std::string Backtrack::GetModeName()
{
	switch(static_cast<BacktrackMode>(Settings::Misc.backtrack))
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

bool Backtrack::GetReal(CTFPlayer *pEntity, LagCompRecord& out)
{
	if (m_records.empty())
		return false;

	auto records = m_records[pEntity->GetIndex()];
	if (records.empty())
		return false;

	LagCompRecord& front = records.front();
	if (!front.IsValid())
		return false;

	out = front;
	return true;
}