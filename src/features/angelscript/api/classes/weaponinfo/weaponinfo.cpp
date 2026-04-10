#include "weaponinfo.h"

#include "../../../../../sdk/classes/weaponbase.h"

void WeaponInfo_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType("WeaponInfo", 0, asOBJ_REF | asOBJ_NOCOUNT);

	engine->RegisterObjectProperty("WeaponInfo", "int m_iWeaponType", asOFFSET(CTFWeaponInfo, m_iWeaponType));
	engine->RegisterObjectProperty("WeaponInfo", "int m_bGrenade", asOFFSET(CTFWeaponInfo, m_bGrenade));
	engine->RegisterObjectProperty("WeaponInfo", "float m_flDamageRadius", asOFFSET(CTFWeaponInfo, m_flDamageRadius));
	engine->RegisterObjectProperty("WeaponInfo", "float m_flPrimerTime", asOFFSET(CTFWeaponInfo, m_flPrimerTime));
	engine->RegisterObjectProperty("WeaponInfo", "bool m_bLowerWeapon", asOFFSET(CTFWeaponInfo, m_bLowerWeapon));
	engine->RegisterObjectProperty("WeaponInfo", "bool m_bSuppressGrenTimer", asOFFSET(CTFWeaponInfo, m_bSuppressGrenTimer));
	engine->RegisterObjectProperty("WeaponInfo", "bool m_bHasTeamSkins_Viewmodel", asOFFSET(CTFWeaponInfo, m_bHasTeamSkins_Viewmodel));
	engine->RegisterObjectProperty("WeaponInfo", "bool m_bHasTeamSkins_Worldmodel", asOFFSET(CTFWeaponInfo, m_bHasTeamSkins_Worldmodel));
	//engine->RegisterObjectProperty("WeaponInfo", "char m_szMuzzleFlashModel[128]", asOFFSET(CTFWeaponInfo, m_szMuzzleFlashModel));
	engine->RegisterObjectProperty("WeaponInfo", "float m_flMuzzleFlashModelDuration", asOFFSET(CTFWeaponInfo, m_flMuzzleFlashModelDuration));
	//engine->RegisterObjectProperty("WeaponInfo", "char m_szMuzzleFlashParticleEffect[128]", asOFFSET(CTFWeaponInfo, m_szMuzzleFlashParticleEffect));
	//engine->RegisterObjectProperty("WeaponInfo", "char m_szTracerEffect[128]", asOFFSET(CTFWeaponInfo, m_szTracerEffect));
	engine->RegisterObjectProperty("WeaponInfo", "bool m_bDoInstantEjectBrass", asOFFSET(CTFWeaponInfo, m_bDoInstantEjectBrass));
	/*engine->RegisterObjectProperty("WeaponInfo", "char m_szBrassModel[128]", asOFFSET(CTFWeaponInfo, m_szBrassModel));
	engine->RegisterObjectProperty("WeaponInfo", "char m_szExplosionSound[128]", asOFFSET(CTFWeaponInfo, m_szExplosionSound));
	engine->RegisterObjectProperty("WeaponInfo", "char m_szExplosionEffect[128]", asOFFSET(CTFWeaponInfo, m_szExplosionEffect));
	engine->RegisterObjectProperty("WeaponInfo", "char m_szExplosionPlayerEffect[128]", asOFFSET(CTFWeaponInfo, m_szExplosionPlayerEffect));
	engine->RegisterObjectProperty("WeaponInfo", "char m_szExplosionWaterEffect[128]", asOFFSET(CTFWeaponInfo, m_szExplosionWaterEffect));*/
	engine->RegisterObjectProperty("WeaponInfo", "bool m_bDontDrop", asOFFSET(CTFWeaponInfo, m_bDontDrop));
}