#pragma once

#include <cstdint>
#include <string.h>
#include <string>

#include "../definitions/icliententity.h"
#include "../definitions/iclientleafsystem.h"
#include "../definitions/types.h"
#include "../definitions/bspflags.h"

#include "../handle_utils.h"
#include "../interfaces/interfaces.h"
#include "../netvars/netvar.h"

#include "../signatures/signatures.h"

ADD_SIG(CBaseEntity_SetAbsOrigin, "client.so", "55 48 89 E5 41 55 41 54 49 89 F4 53 48 89 FB 48 83 EC 08 E8 ? ? ? ? F3 0F 10 83 28 03 00 00")
ADD_SIG(CBaseEntity_SetAbsAngles, "client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 49 89 F4 53 48 89 FB 48 83 EC 68 E8")

#define MULTIPLAYER_BACKUP 90

typedef CHandle<CBaseEntity> EHANDLE;

class CBaseEntity : public IClientEntity
{
public:
	NETVAR(m_flAnimTime, "CBaseEntity->m_flAnimTime", float);
	NETVAR(m_flSimulationTime, "CBaseEntity->m_flSimulationTime", float);
	NETVAR(m_ubInterpolationFrame, "CBaseEntity->m_ubInterpolationFrame", int);
	NETVAR(m_vecOrigin, "CBaseEntity->m_vecOrigin", Vec3);
	NETVAR(m_angRotation, "CBaseEntity->m_angRotation", Vec3);
	NETVAR(m_nModelIndex, "CBaseEntity->m_nModelIndex", int);
	NETVAR(m_fEffects, "CBaseEntity->m_fEffects", int);
	NETVAR(m_nRenderMode, "CBaseEntity->m_nRenderMode", int);
	NETVAR(m_nRenderFX, "CBaseEntity->m_nRenderFX", int);
	NETVAR(m_clrRender, "CBaseEntity->m_clrRender", Color);
	NETVAR(m_iTeamNum, "CBaseEntity->m_iTeamNum", int);
	NETVAR(m_CollisionGroup, "CBaseEntity->m_CollisionGroup", int);
	NETVAR(m_flGravity, "CBaseEntity->m_flGravity", float);
	NETVAR(m_flElasticity, "CBaseEntity->m_flElasticity", float);
	NETVAR(m_flShadowCastDistance, "CBaseEntity->m_flShadowCastDistance", float);
	NETVAR(m_hOwnerEntity, "CBaseEntity->m_hOwnerEntity", EHANDLE);
	NETVAR(m_hEffectEntity, "CBaseEntity->m_hEffectEntity", EHANDLE);
	NETVAR(moveparent, "CBaseEntity->moveparent", int);
	NETVAR(m_iParentAttachment, "CBaseEntity->m_iParentAttachment", int);
	NETVAR(m_Collision, "CBaseEntity->m_Collision", void *);
	NETVAR(m_vecMinsPreScaled, "CBaseEntity->m_vecMinsPreScaled", Vec3);
	NETVAR(m_vecMaxsPreScaled, "CBaseEntity->m_vecMaxsPreScaled", Vec3);
	NETVAR(m_vecMins, "CBaseEntity->m_vecMins", Vec3);
	NETVAR(m_vecMaxs, "CBaseEntity->m_vecMaxs", Vec3);
	NETVAR(m_nSolidType, "CBaseEntity->m_nSolidType", int);
	NETVAR(m_usSolidFlags, "CBaseEntity->m_usSolidFlags", int);
	NETVAR(m_nSurroundType, "CBaseEntity->m_nSurroundType", int);
	NETVAR(m_triggerBloat, "CBaseEntity->m_triggerBloat", int);
	NETVAR(m_bUniformTriggerBloat, "CBaseEntity->m_bUniformTriggerBloat", bool);
	NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, "CBaseEntity->m_vecSpecifiedSurroundingMinsPreScaled", Vec3);
	NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, "CBaseEntity->m_vecSpecifiedSurroundingMaxsPreScaled", Vec3);
	NETVAR(m_vecSpecifiedSurroundingMins, "CBaseEntity->m_vecSpecifiedSurroundingMins", Vec3);
	NETVAR(m_vecSpecifiedSurroundingMaxs, "CBaseEntity->m_vecSpecifiedSurroundingMaxs", Vec3);
	NETVAR(m_iTextureFrameIndex, "CBaseEntity->m_iTextureFrameIndex", int);
	NETVAR(m_PredictableID, "CBaseEntity->m_PredictableID", int);
	NETVAR(m_bIsPlayerSimulated, "CBaseEntity->m_bIsPlayerSimulated", bool);
	NETVAR(m_bSimulatedEveryTick, "CBaseEntity->m_bSimulatedEveryTick", bool);
	NETVAR(m_bAnimatedEveryTick, "CBaseEntity->m_bAnimatedEveryTick", bool);
	NETVAR(m_bAlternateSorting, "CBaseEntity->m_bAlternateSorting", bool);
	NETVAR(m_nModelIndexOverrides, "CBaseEntity->m_nModelIndexOverrides", void *);
	NETVAR(movetype, "CBaseEntity->movetype", int);
	NETVAR(m_flNextAttack, "CBaseCombatCharacter->m_flNextAttack", float);
	NETVAR_OFFSET(m_flRadius, "CBaseEntity->m_usSolidFlags", float, -sizeof(float));
	//NETVAR_OFFSET(m_iEFlags, "CBaseEntity->m_Collision", int, -6);

	bool IsPlayer();
	bool IsWeapon();
	bool IsSentry();
	bool IsDispenser();
	bool IsTeleporter();
	bool IsRobot();

	bool IsBuilding();
	bool IsProjectile();

	int GetIndex();
	Vector GetCenter();

	void CalcAbsVelocity();

	// janky ahh shit
	Vector EstimateAbsVelocity();

	void AddToLeafSystem(RenderGroup_t group);

	bool IsEffectsActive(int nEffects);
	void AddEffects(uint8_t effects);
	void RemoveEffects(uint8_t effects);

	void UpdateVisibility();

	void SetAbsOrigin(const Vec3& absOrigin);
	void SetAbsAngles(const Vec3& absAngle);

	bool ShouldCollide(int collisionGroup, int contentsMask);
	float m_flModelScale();
	void SetCollisionBounds(const Vec3& mins , const Vec3& maxs);
};