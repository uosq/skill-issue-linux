#include "backtrack.h"

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/MaterialManager/materialmanager.h"
#include "../../sdk/helpers/localplayer/localplayer.h"

#include "../../settings/settings.h"

//#include "../ticks/ticks.h"

std::unordered_map<CBaseEntity*, std::deque<LagCompRecord>> Backtrack::m_records = {};
IMaterial* Backtrack::m_mat = nullptr;
bool Backtrack::m_drawing = false;
LagCompRecord* Backtrack::m_current_drawing_record = nullptr;

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

void Backtrack::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd *pCmd)
{
	if (pWeapon->GetWeaponType() == EWeaponType::PROJECTILE
	|| pWeapon->GetWeaponType() == EWeaponType::UNKNOWN)
		return;

	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc::backtrack);
	if (mode == BacktrackMode::NONE || mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID)
		return;

	if (!helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		return;

	Vector eyePos = pLocal->GetEyePos();
	LagCompRecord* bestRecord = nullptr;
	float bestFov = 30.0f;
	Vector viewAngles = pCmd->viewangles;

	for (auto& [entity, records] : m_records)
	{
		if (!entity)
			continue;

		for (auto& record : records)
		{
			//if (interfaces::GlobalVars->curtime - record.simtime >= 0.2f)
			if (!record.IsValid())
				continue;

			Vector aimPos = record.absCenter;
			Vector angle;
			Math::VectorAngles(aimPos - eyePos, angle);

			float fov = Math::CalcFov(viewAngles, angle);
			if (fov < bestFov)
			{
				bestFov = fov;
				bestRecord = &record;
			}
		}
	}

	if (bestRecord == nullptr)
		return;

	int tick = TIME_TO_TICKS(bestRecord->simtime);
	pCmd->tick_count = tick;
}

void Backtrack::Reset()
{
	m_records.clear();
}

void Backtrack::Store(const EntityListEntry& entry)
{
	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc::backtrack);
	if (mode == BacktrackMode::NONE || mode >= BacktrackMode::MAX || mode <= BacktrackMode::INVALID)
		return;

	if (!IsValidPlayer(entry))
		return;

	CTFPlayer* pEntity = static_cast<CTFPlayer*>(entry.ptr);
	if (pEntity == nullptr)
		return;

	auto& records = m_records[pEntity];

	if (!records.empty())
		if (records.back().simtime == pEntity->m_flSimulationTime())
			return;

	matrix3x4 bones[MAXSTUDIOBONES];
	if (!pEntity->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, pEntity->m_flSimulationTime()))
		return;

	LagCompRecord record = {bones, pEntity->m_flSimulationTime(), pEntity->GetCenter(), pEntity->m_angEyeAngles()};
	records.push_front(record);

	//while (!records.empty() && (interfaces::GlobalVars->curtime - records.back().simtime) >= 0.2f)
	while (!records.empty() && !records.back().IsValid())
		records.pop_back();
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

	BacktrackMode mode = static_cast<BacktrackMode>(Settings::Misc::backtrack);
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
		for (auto& [entity, records] : m_records)
		{
			if (records.empty())
				continue;

			auto last = records.back();

			// don't check if its valid
			// as it flickers the shit out of the backtrack
			/*if (!last.IsValid())
				continue;*/

			m_current_drawing_record = &last;
			entity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
		}

		break;
	}
        case BacktrackMode::ALL_RECORDS:
	{
		for (auto& [entity, records] : m_records)
		{
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
	auto records = m_records[pEntity];

	auto origin = LagCompRecord{};
	if (!pEntity->SetupBones(origin.bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, interfaces::GlobalVars->curtime))
		return false;

	origin.absCenter = pEntity->GetCenter();
	origin.simtime = pEntity->m_flSimulationTime();
	origin.viewAngle = pEntity->m_angEyeAngles();

	out.push_back(origin);

	for (auto& record : records)
		out.push_back(record);

	return true;
}

bool Backtrack::IsValidPlayer(const EntityListEntry& entry)
{
	return (entry.flags & (EntityFlags::IsPlayer | EntityFlags::IsEnemy | EntityFlags::IsAlive)) != 0;
}

std::string Backtrack::GetModeName()
{
	switch(static_cast<BacktrackMode>(Settings::Misc::backtrack))
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