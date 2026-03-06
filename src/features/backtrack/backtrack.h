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
		simtime = 0;
		absCenter = {};
		viewAngle = {};
		memset(bones, 0, sizeof(LagCompRecord));
	}

	LagCompRecord(matrix3x4* inBones, float inSimTime, Vector center, Vector viewAngle)
	{
		memcpy(bones, inBones, sizeof(bones));
		simtime = inSimTime;
		absCenter = center;
		this->viewAngle = viewAngle;
	}

	//bool IsValid(CUserCmd* pCmd);
	bool IsValid();

	float simtime;
	Vector absCenter;
	matrix3x4 bones[MAXSTUDIOBONES];
	Vector viewAngle;
};

namespace Backtrack
{
	extern std::unordered_map<int, std::deque<LagCompRecord>> m_records;
	extern IMaterial* m_mat;
	extern bool m_drawing;
	extern LagCompRecord* m_current_drawing_record;

	bool IsValidPlayer(const EntityListEntry& entry);
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd *pCmd);
	void Reset();
	void Store(const EntityListEntry& entry);
	void Init();
	void DoPostScreenSpaceEffects();

	void CleanRecords();
	bool GetRecords(CTFPlayer* pEntity, std::vector<LagCompRecord>& out);
	bool GetReal(CTFPlayer* pEntity, LagCompRecord& out);
	//bool GetClosestRecordFromViewAngle(LagCompRecord* pOut, CBaseEntity* pTarget, const Vector& viewAngle);

	std::string GetModeName();
}