#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../sdk/classes/entity.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/definitions/studio.h"

#include "../entitylist/structs.h"
#include "../feature.h"

struct LagCompRecord
{
	LagCompRecord()
	{
		m_flSimTime	= 0;
		m_vecAbsCenter	= {};
		m_vecViewAngles = {};
		m_vecVelocity	= {};
		m_vecMins	= {};
		m_vecMaxs	= {};
		m_vecAbsOrigin	= {};
		memset(m_Bones, 0, sizeof(m_Bones));
	}

	LagCompRecord(matrix3x4 *pBones, float flSimTime, const Vec3& vecCenter, const Vec3& vecViewAngles, const Vec3& vecVelocity, const Vec3& vecMins, const Vec3& vecMaxs, const Vec3& absOrigin)
	{
		m_flSimTime	= flSimTime;
		m_vecAbsCenter	= vecCenter;
		m_vecViewAngles = vecViewAngles;
		m_vecVelocity	= vecVelocity;
		m_vecMins	= vecMins;
		m_vecMaxs	= vecMaxs;
		memcpy(m_Bones, pBones, sizeof(m_Bones));
	}

	bool IsValid(CUserCmd* pCmd);

	float m_flSimTime;
	Vec3 m_vecAbsCenter;
	matrix3x4 m_Bones[MAXSTUDIOBONES];
	Vec3 m_vecViewAngles;
	Vec3 m_vecVelocity;
	Vec3 m_vecMins;
	Vec3 m_vecMaxs;
	Vec3 m_vecAbsOrigin;
};

class Backtrack
{
public:
	bool IsValidPlayer(const EntityListEntry &entry);
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void Reset();
	void Store();
	void Init();
	void DoPostScreenSpaceEffects();
	float GetInterp();
	float GetLatency();

	void CleanRecords(CUserCmd* pCmd);
	bool GetRecords(CTFPlayer *pEntity, std::vector<LagCompRecord> &out);
	bool GetReal(CTFPlayer *pEntity, LagCompRecord &out);

	std::optional<LagCompRecord> GetDrawingRecord();
	bool IsDrawing();

private:
	std::unordered_map<int, std::deque<LagCompRecord>> m_records{};
	bool m_drawing{false};
	LagCompRecord *m_current_drawing_record{nullptr};
};

DECLARE_FEATURE(Backtrack, backtrack)