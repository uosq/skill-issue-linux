#pragma once

#include "../../sdk/classes/entity.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/definitions/studio.h"

#include "../entitylist/structs.h"

#include <string>
#include <unordered_map>
#include <vector>

struct LagCompRecord
{
	LagCompRecord()
	{
		m_flSimTime	= 0;
		m_vecAbsCenter	= {};
		m_vecViewAngles = {};
		m_vecVelocity	= {};
		memset(m_Bones, 0, sizeof(LagCompRecord));
	}

	LagCompRecord(matrix3x4 *pBones, float flSimTime, Vector vecCenter, Vector vecViewAngles, Vector vecVelocity)
	{
		memcpy(m_Bones, pBones, sizeof(m_Bones));
		m_flSimTime	= flSimTime;
		m_vecAbsCenter	= vecCenter;
		m_vecViewAngles = vecViewAngles;
		m_vecVelocity	= vecVelocity;
	}

	bool IsValid();

	float m_flSimTime;
	Vector m_vecAbsCenter;
	matrix3x4 m_Bones[MAXSTUDIOBONES];
	Vector m_vecViewAngles;
	Vector m_vecVelocity;
};

namespace Backtrack
{
	extern std::unordered_map<int, std::deque<LagCompRecord>> m_records;
	extern IMaterial *m_mat;
	extern bool m_drawing;
	extern LagCompRecord *m_current_drawing_record;

	bool IsValidPlayer(const EntityListEntry &entry);
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void Reset();
	void Store(CTFPlayer *pLocal, const EntityListEntry &entry);
	void Init();
	void DoPostScreenSpaceEffects();
	float GetInterp();
	float GetLatency();

	void CleanRecords();
	bool GetRecords(CTFPlayer *pEntity, std::vector<LagCompRecord> &out);
	bool GetReal(CTFPlayer *pEntity, LagCompRecord &out);

	std::string GetModeName();
} // namespace Backtrack