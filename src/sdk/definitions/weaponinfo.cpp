#include "weaponinfo.h"

FileWeaponInfo_t::FileWeaponInfo_t()
{
}

void FileWeaponInfo_t::Parse(void*, const char*)
{
}

CTFWeaponInfo::CTFWeaponInfo()
{
	m_WeaponData[0].Init();
	m_WeaponData[1].Init();

	m_bGrenade = false;
	m_flDamageRadius = 0.0f;
	m_flPrimerTime = 0.0f;
	m_bSuppressGrenTimer = false;
	m_bLowerWeapon = false;

	m_bHasTeamSkins_Viewmodel = false;
	m_bHasTeamSkins_Worldmodel = false;

	m_szMuzzleFlashModel[0] = '\0';
	m_flMuzzleFlashModelDuration = 0;
	m_szMuzzleFlashParticleEffect[0] = '\0';

	m_szTracerEffect[0] = '\0';

	m_szBrassModel[0] = '\0';
	m_bDoInstantEjectBrass = true;

	m_szExplosionSound[0] = '\0';
	m_szExplosionEffect[0] = '\0';
	m_szExplosionPlayerEffect[0] = '\0';
	m_szExplosionWaterEffect[0] = '\0';

	m_iWeaponType = 0;
}

CTFWeaponInfo::~CTFWeaponInfo()
{
}

void CTFWeaponInfo::Parse(void*, const char*)
{
}