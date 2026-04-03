#include "enums.h"

#include "../../../../sdk/definitions/buttoncode.h"
#include "../../../../sdk/definitions/eclassid.h"

#define REG_ENUM_VAL(engine, enumName, value) engine->RegisterEnumValue(enumName, #value, static_cast<int>(value))

void Enums_Register(asIScriptEngine *engine)
{
	// ETFClassID
	engine->RegisterEnum("ETFClassID");
	engine->RegisterEnumValue("ETFClassID", "CTFWearableRazorback",
				  static_cast<int>(ETFClassID::CTFWearableRazorback));
	engine->RegisterEnumValue("ETFClassID", "CTFWearableDemoShield",
				  static_cast<int>(ETFClassID::CTFWearableDemoShield));
	engine->RegisterEnumValue("ETFClassID", "CTFWearableLevelableItem",
				  static_cast<int>(ETFClassID::CTFWearableLevelableItem));
	engine->RegisterEnumValue("ETFClassID", "CTFWearableCampaignItem",
				  static_cast<int>(ETFClassID::CTFWearableCampaignItem));
	engine->RegisterEnumValue("ETFClassID", "CTFBaseRocket", static_cast<int>(ETFClassID::CTFBaseRocket));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponBaseMerasmusGrenade",
				  static_cast<int>(ETFClassID::CTFWeaponBaseMerasmusGrenade));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponBaseMelee", static_cast<int>(ETFClassID::CTFWeaponBaseMelee));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponBaseGun", static_cast<int>(ETFClassID::CTFWeaponBaseGun));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponBaseGrenadeProj",
				  static_cast<int>(ETFClassID::CTFWeaponBaseGrenadeProj));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponBase", static_cast<int>(ETFClassID::CTFWeaponBase));
	engine->RegisterEnumValue("ETFClassID", "CTFWearableRobotArm",
				  static_cast<int>(ETFClassID::CTFWearableRobotArm));
	engine->RegisterEnumValue("ETFClassID", "CTFRobotArm", static_cast<int>(ETFClassID::CTFRobotArm));
	engine->RegisterEnumValue("ETFClassID", "CTFWrench", static_cast<int>(ETFClassID::CTFWrench));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_ThrowableBreadMonster",
				  static_cast<int>(ETFClassID::CTFProjectile_ThrowableBreadMonster));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_ThrowableBrick",
				  static_cast<int>(ETFClassID::CTFProjectile_ThrowableBrick));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_ThrowableRepel",
				  static_cast<int>(ETFClassID::CTFProjectile_ThrowableRepel));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_Throwable",
				  static_cast<int>(ETFClassID::CTFProjectile_Throwable));
	engine->RegisterEnumValue("ETFClassID", "CTFThrowable", static_cast<int>(ETFClassID::CTFThrowable));
	engine->RegisterEnumValue("ETFClassID", "CTFSyringeGun", static_cast<int>(ETFClassID::CTFSyringeGun));
	engine->RegisterEnumValue("ETFClassID", "CTFKatana", static_cast<int>(ETFClassID::CTFKatana));
	engine->RegisterEnumValue("ETFClassID", "CTFSword", static_cast<int>(ETFClassID::CTFSword));
	engine->RegisterEnumValue("ETFClassID", "CSniperDot", static_cast<int>(ETFClassID::CSniperDot));
	engine->RegisterEnumValue("ETFClassID", "CTFSniperRifleClassic",
				  static_cast<int>(ETFClassID::CTFSniperRifleClassic));
	engine->RegisterEnumValue("ETFClassID", "CTFSniperRifleDecap",
				  static_cast<int>(ETFClassID::CTFSniperRifleDecap));
	engine->RegisterEnumValue("ETFClassID", "CTFSniperRifle", static_cast<int>(ETFClassID::CTFSniperRifle));
	engine->RegisterEnumValue("ETFClassID", "CTFChargedSMG", static_cast<int>(ETFClassID::CTFChargedSMG));
	engine->RegisterEnumValue("ETFClassID", "CTFSMG", static_cast<int>(ETFClassID::CTFSMG));
	engine->RegisterEnumValue("ETFClassID", "CTFSlap", static_cast<int>(ETFClassID::CTFSlap));
	engine->RegisterEnumValue("ETFClassID", "CTFShovel", static_cast<int>(ETFClassID::CTFShovel));
	engine->RegisterEnumValue("ETFClassID", "CTFShotgunBuildingRescue",
				  static_cast<int>(ETFClassID::CTFShotgunBuildingRescue));
	engine->RegisterEnumValue("ETFClassID", "CTFPEPBrawlerBlaster",
				  static_cast<int>(ETFClassID::CTFPEPBrawlerBlaster));
	engine->RegisterEnumValue("ETFClassID", "CTFSodaPopper", static_cast<int>(ETFClassID::CTFSodaPopper));
	engine->RegisterEnumValue("ETFClassID", "CTFShotgun_Revenge", static_cast<int>(ETFClassID::CTFShotgun_Revenge));
	engine->RegisterEnumValue("ETFClassID", "CTFScatterGun", static_cast<int>(ETFClassID::CTFScatterGun));
	engine->RegisterEnumValue("ETFClassID", "CTFShotgun_Pyro", static_cast<int>(ETFClassID::CTFShotgun_Pyro));
	engine->RegisterEnumValue("ETFClassID", "CTFShotgun_HWG", static_cast<int>(ETFClassID::CTFShotgun_HWG));
	engine->RegisterEnumValue("ETFClassID", "CTFShotgun_Soldier", static_cast<int>(ETFClassID::CTFShotgun_Soldier));
	engine->RegisterEnumValue("ETFClassID", "CTFShotgun", static_cast<int>(ETFClassID::CTFShotgun));
	engine->RegisterEnumValue("ETFClassID", "CTFRocketPack", static_cast<int>(ETFClassID::CTFRocketPack));
	engine->RegisterEnumValue("ETFClassID", "CTFCrossbow", static_cast<int>(ETFClassID::CTFCrossbow));
	engine->RegisterEnumValue("ETFClassID", "CTFRocketLauncher_Mortar",
				  static_cast<int>(ETFClassID::CTFRocketLauncher_Mortar));
	engine->RegisterEnumValue("ETFClassID", "CTFRocketLauncher_AirStrike",
				  static_cast<int>(ETFClassID::CTFRocketLauncher_AirStrike));
	engine->RegisterEnumValue("ETFClassID", "CTFRocketLauncher_DirectHit",
				  static_cast<int>(ETFClassID::CTFRocketLauncher_DirectHit));
	engine->RegisterEnumValue("ETFClassID", "CTFRocketLauncher", static_cast<int>(ETFClassID::CTFRocketLauncher));
	engine->RegisterEnumValue("ETFClassID", "CTFRevolver", static_cast<int>(ETFClassID::CTFRevolver));
	engine->RegisterEnumValue("ETFClassID", "CTFDRGPomson", static_cast<int>(ETFClassID::CTFDRGPomson));
	engine->RegisterEnumValue("ETFClassID", "CTFRaygun", static_cast<int>(ETFClassID::CTFRaygun));
	engine->RegisterEnumValue("ETFClassID", "CTFPistol_ScoutSecondary",
				  static_cast<int>(ETFClassID::CTFPistol_ScoutSecondary));
	engine->RegisterEnumValue("ETFClassID", "CTFPistol_ScoutPrimary",
				  static_cast<int>(ETFClassID::CTFPistol_ScoutPrimary));
	engine->RegisterEnumValue("ETFClassID", "CTFPistol_Scout", static_cast<int>(ETFClassID::CTFPistol_Scout));
	engine->RegisterEnumValue("ETFClassID", "CTFPistol", static_cast<int>(ETFClassID::CTFPistol));
	engine->RegisterEnumValue("ETFClassID", "CTFPipebombLauncher",
				  static_cast<int>(ETFClassID::CTFPipebombLauncher));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponPDA_Spy", static_cast<int>(ETFClassID::CTFWeaponPDA_Spy));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponPDA_Engineer_Destroy",
				  static_cast<int>(ETFClassID::CTFWeaponPDA_Engineer_Destroy));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponPDA_Engineer_Build",
				  static_cast<int>(ETFClassID::CTFWeaponPDA_Engineer_Build));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponPDAExpansion_Teleporter",
				  static_cast<int>(ETFClassID::CTFWeaponPDAExpansion_Teleporter));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponPDAExpansion_Dispenser",
				  static_cast<int>(ETFClassID::CTFWeaponPDAExpansion_Dispenser));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponPDA", static_cast<int>(ETFClassID::CTFWeaponPDA));
	engine->RegisterEnumValue("ETFClassID", "CTFParticleCannon", static_cast<int>(ETFClassID::CTFParticleCannon));
	engine->RegisterEnumValue("ETFClassID", "CTFParachute_Secondary",
				  static_cast<int>(ETFClassID::CTFParachute_Secondary));
	engine->RegisterEnumValue("ETFClassID", "CTFParachute_Primary",
				  static_cast<int>(ETFClassID::CTFParachute_Primary));
	engine->RegisterEnumValue("ETFClassID", "CTFParachute", static_cast<int>(ETFClassID::CTFParachute));
	engine->RegisterEnumValue("ETFClassID", "CTFMinigun", static_cast<int>(ETFClassID::CTFMinigun));
	engine->RegisterEnumValue("ETFClassID", "CTFMedigunShield", static_cast<int>(ETFClassID::CTFMedigunShield));
	engine->RegisterEnumValue("ETFClassID", "CWeaponMedigun", static_cast<int>(ETFClassID::CWeaponMedigun));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_MechanicalArmOrb",
				  static_cast<int>(ETFClassID::CTFProjectile_MechanicalArmOrb));
	engine->RegisterEnumValue("ETFClassID", "CTFMechanicalArm", static_cast<int>(ETFClassID::CTFMechanicalArm));
	engine->RegisterEnumValue("ETFClassID", "CTFLunchBox_Drink", static_cast<int>(ETFClassID::CTFLunchBox_Drink));
	engine->RegisterEnumValue("ETFClassID", "CTFLunchBox", static_cast<int>(ETFClassID::CTFLunchBox));
	engine->RegisterEnumValue("ETFClassID", "CLaserDot", static_cast<int>(ETFClassID::CLaserDot));
	engine->RegisterEnumValue("ETFClassID", "CTFLaserPointer", static_cast<int>(ETFClassID::CTFLaserPointer));
	engine->RegisterEnumValue("ETFClassID", "CTFKnife", static_cast<int>(ETFClassID::CTFKnife));
	engine->RegisterEnumValue("ETFClassID", "CTFGasManager", static_cast<int>(ETFClassID::CTFGasManager));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_JarGas",
				  static_cast<int>(ETFClassID::CTFProjectile_JarGas));
	engine->RegisterEnumValue("ETFClassID", "CTFJarGas", static_cast<int>(ETFClassID::CTFJarGas));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_Cleaver",
				  static_cast<int>(ETFClassID::CTFProjectile_Cleaver));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_JarMilk",
				  static_cast<int>(ETFClassID::CTFProjectile_JarMilk));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_Jar", static_cast<int>(ETFClassID::CTFProjectile_Jar));
	engine->RegisterEnumValue("ETFClassID", "CTFCleaver", static_cast<int>(ETFClassID::CTFCleaver));
	engine->RegisterEnumValue("ETFClassID", "CTFJarMilk", static_cast<int>(ETFClassID::CTFJarMilk));
	engine->RegisterEnumValue("ETFClassID", "CTFJar", static_cast<int>(ETFClassID::CTFJar));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponInvis", static_cast<int>(ETFClassID::CTFWeaponInvis));
	engine->RegisterEnumValue("ETFClassID", "CTFCannon", static_cast<int>(ETFClassID::CTFCannon));
	engine->RegisterEnumValue("ETFClassID", "CTFGrenadeLauncher", static_cast<int>(ETFClassID::CTFGrenadeLauncher));
	engine->RegisterEnumValue("ETFClassID", "CTFGrenadePipebombProjectile",
				  static_cast<int>(ETFClassID::CTFGrenadePipebombProjectile));
	engine->RegisterEnumValue("ETFClassID", "CTFGrapplingHook", static_cast<int>(ETFClassID::CTFGrapplingHook));
	engine->RegisterEnumValue("ETFClassID", "CTFFlareGun_Revenge",
				  static_cast<int>(ETFClassID::CTFFlareGun_Revenge));
	engine->RegisterEnumValue("ETFClassID", "CTFFlareGun", static_cast<int>(ETFClassID::CTFFlareGun));
	engine->RegisterEnumValue("ETFClassID", "CTFFlameRocket", static_cast<int>(ETFClassID::CTFFlameRocket));
	engine->RegisterEnumValue("ETFClassID", "CTFFlameThrower", static_cast<int>(ETFClassID::CTFFlameThrower));
	engine->RegisterEnumValue("ETFClassID", "CTFFists", static_cast<int>(ETFClassID::CTFFists));
	engine->RegisterEnumValue("ETFClassID", "CTFFireAxe", static_cast<int>(ETFClassID::CTFFireAxe));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponFlameBall", static_cast<int>(ETFClassID::CTFWeaponFlameBall));
	engine->RegisterEnumValue("ETFClassID", "CTFCompoundBow", static_cast<int>(ETFClassID::CTFCompoundBow));
	engine->RegisterEnumValue("ETFClassID", "CTFClub", static_cast<int>(ETFClassID::CTFClub));
	engine->RegisterEnumValue("ETFClassID", "CTFBuffItem", static_cast<int>(ETFClassID::CTFBuffItem));
	engine->RegisterEnumValue("ETFClassID", "CTFStickBomb", static_cast<int>(ETFClassID::CTFStickBomb));
	engine->RegisterEnumValue("ETFClassID", "CTFBreakableSign", static_cast<int>(ETFClassID::CTFBreakableSign));
	engine->RegisterEnumValue("ETFClassID", "CTFBottle", static_cast<int>(ETFClassID::CTFBottle));
	engine->RegisterEnumValue("ETFClassID", "CTFBreakableMelee", static_cast<int>(ETFClassID::CTFBreakableMelee));
	engine->RegisterEnumValue("ETFClassID", "CTFBonesaw", static_cast<int>(ETFClassID::CTFBonesaw));
	engine->RegisterEnumValue("ETFClassID", "CTFBall_Ornament", static_cast<int>(ETFClassID::CTFBall_Ornament));
	engine->RegisterEnumValue("ETFClassID", "CTFStunBall", static_cast<int>(ETFClassID::CTFStunBall));
	engine->RegisterEnumValue("ETFClassID", "CTFBat_Giftwrap", static_cast<int>(ETFClassID::CTFBat_Giftwrap));
	engine->RegisterEnumValue("ETFClassID", "CTFBat_Wood", static_cast<int>(ETFClassID::CTFBat_Wood));
	engine->RegisterEnumValue("ETFClassID", "CTFBat_Fish", static_cast<int>(ETFClassID::CTFBat_Fish));
	engine->RegisterEnumValue("ETFClassID", "CTFBat", static_cast<int>(ETFClassID::CTFBat));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_EnergyRing",
				  static_cast<int>(ETFClassID::CTFProjectile_EnergyRing));
	engine->RegisterEnumValue("ETFClassID", "CTFDroppedWeapon", static_cast<int>(ETFClassID::CTFDroppedWeapon));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponSapper", static_cast<int>(ETFClassID::CTFWeaponSapper));
	engine->RegisterEnumValue("ETFClassID", "CTFWeaponBuilder", static_cast<int>(ETFClassID::CTFWeaponBuilder));
	engine->RegisterEnumValue("ETFClassID", "C_TFWeaponBuilder", static_cast<int>(ETFClassID::C_TFWeaponBuilder));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_Rocket",
				  static_cast<int>(ETFClassID::CTFProjectile_Rocket));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_Flare",
				  static_cast<int>(ETFClassID::CTFProjectile_Flare));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_EnergyBall",
				  static_cast<int>(ETFClassID::CTFProjectile_EnergyBall));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_GrapplingHook",
				  static_cast<int>(ETFClassID::CTFProjectile_GrapplingHook));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_HealingBolt",
				  static_cast<int>(ETFClassID::CTFProjectile_HealingBolt));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_Arrow",
				  static_cast<int>(ETFClassID::CTFProjectile_Arrow));
	engine->RegisterEnumValue("ETFClassID", "CMannVsMachineStats",
				  static_cast<int>(ETFClassID::CMannVsMachineStats));
	engine->RegisterEnumValue("ETFClassID", "CTFTankBoss", static_cast<int>(ETFClassID::CTFTankBoss));
	engine->RegisterEnumValue("ETFClassID", "CTFBaseBoss", static_cast<int>(ETFClassID::CTFBaseBoss));
	engine->RegisterEnumValue("ETFClassID", "CBossAlpha", static_cast<int>(ETFClassID::CBossAlpha));
	engine->RegisterEnumValue("ETFClassID", "NextBotCombatCharacter",
				  static_cast<int>(ETFClassID::NextBotCombatCharacter));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellKartBats",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellKartBats));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellKartOrb",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellKartOrb));
	engine->RegisterEnumValue("ETFClassID", "CTFHellZap", static_cast<int>(ETFClassID::CTFHellZap));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellLightningOrb",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellLightningOrb));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellTransposeTeleport",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellTransposeTeleport));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellMeteorShower",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellMeteorShower));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellSpawnBoss",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellSpawnBoss));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellMirv",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellMirv));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellPumpkin",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellPumpkin));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellSpawnHorde",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellSpawnHorde));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellSpawnZombie",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellSpawnZombie));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellBats",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellBats));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SpellFireball",
				  static_cast<int>(ETFClassID::CTFProjectile_SpellFireball));
	engine->RegisterEnumValue("ETFClassID", "CTFSpellBook", static_cast<int>(ETFClassID::CTFSpellBook));
	engine->RegisterEnumValue("ETFClassID", "CHightower_TeleportVortex",
				  static_cast<int>(ETFClassID::CHightower_TeleportVortex));
	engine->RegisterEnumValue("ETFClassID", "CTeleportVortex", static_cast<int>(ETFClassID::CTeleportVortex));
	engine->RegisterEnumValue("ETFClassID", "CZombie", static_cast<int>(ETFClassID::CZombie));
	engine->RegisterEnumValue("ETFClassID", "CMerasmusDancer", static_cast<int>(ETFClassID::CMerasmusDancer));
	engine->RegisterEnumValue("ETFClassID", "CMerasmus", static_cast<int>(ETFClassID::CMerasmus));
	engine->RegisterEnumValue("ETFClassID", "CHeadlessHatman", static_cast<int>(ETFClassID::CHeadlessHatman));
	engine->RegisterEnumValue("ETFClassID", "CEyeballBoss", static_cast<int>(ETFClassID::CEyeballBoss));
	engine->RegisterEnumValue("ETFClassID", "CTFBotHintEngineerNest",
				  static_cast<int>(ETFClassID::CTFBotHintEngineerNest));
	engine->RegisterEnumValue("ETFClassID", "CBotNPCMinion", static_cast<int>(ETFClassID::CBotNPCMinion));
	engine->RegisterEnumValue("ETFClassID", "CBotNPC", static_cast<int>(ETFClassID::CBotNPC));
	engine->RegisterEnumValue("ETFClassID", "CPasstimeGun", static_cast<int>(ETFClassID::CPasstimeGun));
	engine->RegisterEnumValue("ETFClassID", "CTFViewModel", static_cast<int>(ETFClassID::CTFViewModel));
	engine->RegisterEnumValue("ETFClassID", "CRobotDispenser", static_cast<int>(ETFClassID::CRobotDispenser));
	engine->RegisterEnumValue("ETFClassID", "CTFRobotDestruction_Robot",
				  static_cast<int>(ETFClassID::CTFRobotDestruction_Robot));
	engine->RegisterEnumValue("ETFClassID", "CTFReviveMarker", static_cast<int>(ETFClassID::CTFReviveMarker));
	engine->RegisterEnumValue("ETFClassID", "CTFPumpkinBomb", static_cast<int>(ETFClassID::CTFPumpkinBomb));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_BallOfFire",
				  static_cast<int>(ETFClassID::CTFProjectile_BallOfFire));
	engine->RegisterEnumValue("ETFClassID", "CTFBaseProjectile", static_cast<int>(ETFClassID::CTFBaseProjectile));
	engine->RegisterEnumValue("ETFClassID", "CTFPointManager", static_cast<int>(ETFClassID::CTFPointManager));
	engine->RegisterEnumValue("ETFClassID", "CBaseObjectUpgrade", static_cast<int>(ETFClassID::CBaseObjectUpgrade));
	engine->RegisterEnumValue("ETFClassID", "CTFRobotDestructionLogic",
				  static_cast<int>(ETFClassID::CTFRobotDestructionLogic));
	engine->RegisterEnumValue("ETFClassID", "CTFRobotDestruction_RobotGroup",
				  static_cast<int>(ETFClassID::CTFRobotDestruction_RobotGroup));
	engine->RegisterEnumValue("ETFClassID", "CTFRobotDestruction_RobotSpawn",
				  static_cast<int>(ETFClassID::CTFRobotDestruction_RobotSpawn));
	engine->RegisterEnumValue("ETFClassID", "CTFPlayerDestructionLogic",
				  static_cast<int>(ETFClassID::CTFPlayerDestructionLogic));
	engine->RegisterEnumValue("ETFClassID", "CPlayerDestructionDispenser",
				  static_cast<int>(ETFClassID::CPlayerDestructionDispenser));
	engine->RegisterEnumValue("ETFClassID", "CTFMinigameLogic", static_cast<int>(ETFClassID::CTFMinigameLogic));
	engine->RegisterEnumValue("ETFClassID", "CTFHalloweenMinigame_FallingPlatforms",
				  static_cast<int>(ETFClassID::CTFHalloweenMinigame_FallingPlatforms));
	engine->RegisterEnumValue("ETFClassID", "CTFHalloweenMinigame",
				  static_cast<int>(ETFClassID::CTFHalloweenMinigame));
	engine->RegisterEnumValue("ETFClassID", "CTFMiniGame", static_cast<int>(ETFClassID::CTFMiniGame));
	engine->RegisterEnumValue("ETFClassID", "CTFPowerupBottle", static_cast<int>(ETFClassID::CTFPowerupBottle));
	engine->RegisterEnumValue("ETFClassID", "CTFItem", static_cast<int>(ETFClassID::CTFItem));
	engine->RegisterEnumValue("ETFClassID", "CHalloweenSoulPack", static_cast<int>(ETFClassID::CHalloweenSoulPack));
	engine->RegisterEnumValue("ETFClassID", "CTFGenericBomb", static_cast<int>(ETFClassID::CTFGenericBomb));
	engine->RegisterEnumValue("ETFClassID", "CBonusRoundLogic", static_cast<int>(ETFClassID::CBonusRoundLogic));
	engine->RegisterEnumValue("ETFClassID", "CTFGameRulesProxy", static_cast<int>(ETFClassID::CTFGameRulesProxy));
	engine->RegisterEnumValue("ETFClassID", "CTETFParticleEffect",
				  static_cast<int>(ETFClassID::CTETFParticleEffect));
	engine->RegisterEnumValue("ETFClassID", "CTETFExplosion", static_cast<int>(ETFClassID::CTETFExplosion));
	engine->RegisterEnumValue("ETFClassID", "CTETFBlood", static_cast<int>(ETFClassID::CTETFBlood));
	engine->RegisterEnumValue("ETFClassID", "CTFFlameManager", static_cast<int>(ETFClassID::CTFFlameManager));
	engine->RegisterEnumValue("ETFClassID", "CHalloweenGiftPickup",
				  static_cast<int>(ETFClassID::CHalloweenGiftPickup));
	engine->RegisterEnumValue("ETFClassID", "CBonusDuckPickup", static_cast<int>(ETFClassID::CBonusDuckPickup));
	engine->RegisterEnumValue("ETFClassID", "CHalloweenPickup", static_cast<int>(ETFClassID::CHalloweenPickup));
	engine->RegisterEnumValue("ETFClassID", "CCaptureFlagReturnIcon",
				  static_cast<int>(ETFClassID::CCaptureFlagReturnIcon));
	engine->RegisterEnumValue("ETFClassID", "CCaptureFlag", static_cast<int>(ETFClassID::CCaptureFlag));
	engine->RegisterEnumValue("ETFClassID", "CBonusPack", static_cast<int>(ETFClassID::CBonusPack));
	engine->RegisterEnumValue("ETFClassID", "CTFTeam", static_cast<int>(ETFClassID::CTFTeam));
	engine->RegisterEnumValue("ETFClassID", "CTFTauntProp", static_cast<int>(ETFClassID::CTFTauntProp));
	engine->RegisterEnumValue("ETFClassID", "CTFPlayerResource", static_cast<int>(ETFClassID::CTFPlayerResource));
	engine->RegisterEnumValue("ETFClassID", "CTFPlayer", static_cast<int>(ETFClassID::CTFPlayer));
	engine->RegisterEnumValue("ETFClassID", "CTFRagdoll", static_cast<int>(ETFClassID::CTFRagdoll));
	engine->RegisterEnumValue("ETFClassID", "CTEPlayerAnimEvent", static_cast<int>(ETFClassID::CTEPlayerAnimEvent));
	engine->RegisterEnumValue("ETFClassID", "CTFPasstimeLogic", static_cast<int>(ETFClassID::CTFPasstimeLogic));
	engine->RegisterEnumValue("ETFClassID", "CPasstimeBall", static_cast<int>(ETFClassID::CPasstimeBall));
	engine->RegisterEnumValue("ETFClassID", "CTFObjectiveResource",
				  static_cast<int>(ETFClassID::CTFObjectiveResource));
	engine->RegisterEnumValue("ETFClassID", "CTFGlow", static_cast<int>(ETFClassID::CTFGlow));
	engine->RegisterEnumValue("ETFClassID", "CTEFireBullets", static_cast<int>(ETFClassID::CTEFireBullets));
	engine->RegisterEnumValue("ETFClassID", "CTFBuffBanner", static_cast<int>(ETFClassID::CTFBuffBanner));
	engine->RegisterEnumValue("ETFClassID", "CTFAmmoPack", static_cast<int>(ETFClassID::CTFAmmoPack));
	engine->RegisterEnumValue("ETFClassID", "CObjectTeleporter", static_cast<int>(ETFClassID::CObjectTeleporter));
	engine->RegisterEnumValue("ETFClassID", "CObjectSentrygun", static_cast<int>(ETFClassID::CObjectSentrygun));
	engine->RegisterEnumValue("ETFClassID", "CTFProjectile_SentryRocket",
				  static_cast<int>(ETFClassID::CTFProjectile_SentryRocket));
	engine->RegisterEnumValue("ETFClassID", "CObjectSapper", static_cast<int>(ETFClassID::CObjectSapper));
	engine->RegisterEnumValue("ETFClassID", "CObjectCartDispenser",
				  static_cast<int>(ETFClassID::CObjectCartDispenser));
	engine->RegisterEnumValue("ETFClassID", "CObjectDispenser", static_cast<int>(ETFClassID::CObjectDispenser));
	engine->RegisterEnumValue("ETFClassID", "CMonsterResource", static_cast<int>(ETFClassID::CMonsterResource));
	engine->RegisterEnumValue("ETFClassID", "CFuncRespawnRoomVisualizer",
				  static_cast<int>(ETFClassID::CFuncRespawnRoomVisualizer));
	engine->RegisterEnumValue("ETFClassID", "CFuncRespawnRoom", static_cast<int>(ETFClassID::CFuncRespawnRoom));
	engine->RegisterEnumValue("ETFClassID", "CFuncPasstimeGoal", static_cast<int>(ETFClassID::CFuncPasstimeGoal));
	engine->RegisterEnumValue("ETFClassID", "CFuncForceField", static_cast<int>(ETFClassID::CFuncForceField));
	engine->RegisterEnumValue("ETFClassID", "CCaptureZone", static_cast<int>(ETFClassID::CCaptureZone));
	engine->RegisterEnumValue("ETFClassID", "CCurrencyPack", static_cast<int>(ETFClassID::CCurrencyPack));
	engine->RegisterEnumValue("ETFClassID", "CBaseObject", static_cast<int>(ETFClassID::CBaseObject));
	engine->RegisterEnumValue("ETFClassID", "CTestTraceline", static_cast<int>(ETFClassID::CTestTraceline));
	engine->RegisterEnumValue("ETFClassID", "CTEWorldDecal", static_cast<int>(ETFClassID::CTEWorldDecal));
	engine->RegisterEnumValue("ETFClassID", "CTESpriteSpray", static_cast<int>(ETFClassID::CTESpriteSpray));
	engine->RegisterEnumValue("ETFClassID", "CTESprite", static_cast<int>(ETFClassID::CTESprite));
	engine->RegisterEnumValue("ETFClassID", "CTESparks", static_cast<int>(ETFClassID::CTESparks));
	engine->RegisterEnumValue("ETFClassID", "CTESmoke", static_cast<int>(ETFClassID::CTESmoke));
	engine->RegisterEnumValue("ETFClassID", "CTEShowLine", static_cast<int>(ETFClassID::CTEShowLine));
	engine->RegisterEnumValue("ETFClassID", "CTEProjectedDecal", static_cast<int>(ETFClassID::CTEProjectedDecal));
	engine->RegisterEnumValue("ETFClassID", "CTEPlayerDecal", static_cast<int>(ETFClassID::CTEPlayerDecal));
	engine->RegisterEnumValue("ETFClassID", "CTEPhysicsProp", static_cast<int>(ETFClassID::CTEPhysicsProp));
	engine->RegisterEnumValue("ETFClassID", "CTEParticleSystem", static_cast<int>(ETFClassID::CTEParticleSystem));
	engine->RegisterEnumValue("ETFClassID", "CTEMuzzleFlash", static_cast<int>(ETFClassID::CTEMuzzleFlash));
	engine->RegisterEnumValue("ETFClassID", "CTELargeFunnel", static_cast<int>(ETFClassID::CTELargeFunnel));
	engine->RegisterEnumValue("ETFClassID", "CTEKillPlayerAttachments",
				  static_cast<int>(ETFClassID::CTEKillPlayerAttachments));
	engine->RegisterEnumValue("ETFClassID", "CTEImpact", static_cast<int>(ETFClassID::CTEImpact));
	engine->RegisterEnumValue("ETFClassID", "CTEGlowSprite", static_cast<int>(ETFClassID::CTEGlowSprite));
	engine->RegisterEnumValue("ETFClassID", "CTEShatterSurface", static_cast<int>(ETFClassID::CTEShatterSurface));
	engine->RegisterEnumValue("ETFClassID", "CTEFootprintDecal", static_cast<int>(ETFClassID::CTEFootprintDecal));
	engine->RegisterEnumValue("ETFClassID", "CTEFizz", static_cast<int>(ETFClassID::CTEFizz));
	engine->RegisterEnumValue("ETFClassID", "CTEExplosion", static_cast<int>(ETFClassID::CTEExplosion));
	engine->RegisterEnumValue("ETFClassID", "CTEEnergySplash", static_cast<int>(ETFClassID::CTEEnergySplash));
	engine->RegisterEnumValue("ETFClassID", "CTEEffectDispatch", static_cast<int>(ETFClassID::CTEEffectDispatch));
	engine->RegisterEnumValue("ETFClassID", "CTEDynamicLight", static_cast<int>(ETFClassID::CTEDynamicLight));
	engine->RegisterEnumValue("ETFClassID", "CTEDecal", static_cast<int>(ETFClassID::CTEDecal));
	engine->RegisterEnumValue("ETFClassID", "CTEClientProjectile",
				  static_cast<int>(ETFClassID::CTEClientProjectile));
	engine->RegisterEnumValue("ETFClassID", "CTEBubbleTrail", static_cast<int>(ETFClassID::CTEBubbleTrail));
	engine->RegisterEnumValue("ETFClassID", "CTEBubbles", static_cast<int>(ETFClassID::CTEBubbles));
	engine->RegisterEnumValue("ETFClassID", "CTEBSPDecal", static_cast<int>(ETFClassID::CTEBSPDecal));
	engine->RegisterEnumValue("ETFClassID", "CTEBreakModel", static_cast<int>(ETFClassID::CTEBreakModel));
	engine->RegisterEnumValue("ETFClassID", "CTEBloodStream", static_cast<int>(ETFClassID::CTEBloodStream));
	engine->RegisterEnumValue("ETFClassID", "CTEBloodSprite", static_cast<int>(ETFClassID::CTEBloodSprite));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamSpline", static_cast<int>(ETFClassID::CTEBeamSpline));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamRingPoint", static_cast<int>(ETFClassID::CTEBeamRingPoint));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamRing", static_cast<int>(ETFClassID::CTEBeamRing));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamPoints", static_cast<int>(ETFClassID::CTEBeamPoints));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamLaser", static_cast<int>(ETFClassID::CTEBeamLaser));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamFollow", static_cast<int>(ETFClassID::CTEBeamFollow));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamEnts", static_cast<int>(ETFClassID::CTEBeamEnts));
	engine->RegisterEnumValue("ETFClassID", "CTEBeamEntPoint", static_cast<int>(ETFClassID::CTEBeamEntPoint));
	engine->RegisterEnumValue("ETFClassID", "CTEBaseBeam", static_cast<int>(ETFClassID::CTEBaseBeam));
	engine->RegisterEnumValue("ETFClassID", "CTEArmorRicochet", static_cast<int>(ETFClassID::CTEArmorRicochet));
	engine->RegisterEnumValue("ETFClassID", "CTEMetalSparks", static_cast<int>(ETFClassID::CTEMetalSparks));
	engine->RegisterEnumValue("ETFClassID", "CSteamJet", static_cast<int>(ETFClassID::CSteamJet));
	engine->RegisterEnumValue("ETFClassID", "CSmokeStack", static_cast<int>(ETFClassID::CSmokeStack));
	engine->RegisterEnumValue("ETFClassID", "DustTrail", static_cast<int>(ETFClassID::DustTrail));
	engine->RegisterEnumValue("ETFClassID", "CFireTrail", static_cast<int>(ETFClassID::CFireTrail));
	engine->RegisterEnumValue("ETFClassID", "SporeTrail", static_cast<int>(ETFClassID::SporeTrail));
	engine->RegisterEnumValue("ETFClassID", "SporeExplosion", static_cast<int>(ETFClassID::SporeExplosion));
	engine->RegisterEnumValue("ETFClassID", "RocketTrail", static_cast<int>(ETFClassID::RocketTrail));
	engine->RegisterEnumValue("ETFClassID", "SmokeTrail", static_cast<int>(ETFClassID::SmokeTrail));
	engine->RegisterEnumValue("ETFClassID", "CPropVehicleDriveable",
				  static_cast<int>(ETFClassID::CPropVehicleDriveable));
	engine->RegisterEnumValue("ETFClassID", "ParticleSmokeGrenade",
				  static_cast<int>(ETFClassID::ParticleSmokeGrenade));
	engine->RegisterEnumValue("ETFClassID", "CParticleFire", static_cast<int>(ETFClassID::CParticleFire));
	engine->RegisterEnumValue("ETFClassID", "MovieExplosion", static_cast<int>(ETFClassID::MovieExplosion));
	engine->RegisterEnumValue("ETFClassID", "CTEGaussExplosion", static_cast<int>(ETFClassID::CTEGaussExplosion));
	engine->RegisterEnumValue("ETFClassID", "CEnvQuadraticBeam", static_cast<int>(ETFClassID::CEnvQuadraticBeam));
	engine->RegisterEnumValue("ETFClassID", "CEmbers", static_cast<int>(ETFClassID::CEmbers));
	engine->RegisterEnumValue("ETFClassID", "CEnvWind", static_cast<int>(ETFClassID::CEnvWind));
	engine->RegisterEnumValue("ETFClassID", "CPrecipitation", static_cast<int>(ETFClassID::CPrecipitation));
	engine->RegisterEnumValue("ETFClassID", "CBaseTempEntity", static_cast<int>(ETFClassID::CBaseTempEntity));
	engine->RegisterEnumValue("ETFClassID", "CWeaponIFMSteadyCam",
				  static_cast<int>(ETFClassID::CWeaponIFMSteadyCam));
	engine->RegisterEnumValue("ETFClassID", "CWeaponIFMBaseCamera",
				  static_cast<int>(ETFClassID::CWeaponIFMBaseCamera));
	engine->RegisterEnumValue("ETFClassID", "CWeaponIFMBase", static_cast<int>(ETFClassID::CWeaponIFMBase));
	engine->RegisterEnumValue("ETFClassID", "CTFWearableVM", static_cast<int>(ETFClassID::CTFWearableVM));
	engine->RegisterEnumValue("ETFClassID", "CTFWearable", static_cast<int>(ETFClassID::CTFWearable));
	engine->RegisterEnumValue("ETFClassID", "CTFWearableItem", static_cast<int>(ETFClassID::CTFWearableItem));
	engine->RegisterEnumValue("ETFClassID", "CEconWearable", static_cast<int>(ETFClassID::CEconWearable));
	engine->RegisterEnumValue("ETFClassID", "CBaseAttributableItem",
				  static_cast<int>(ETFClassID::CBaseAttributableItem));
	engine->RegisterEnumValue("ETFClassID", "CEconEntity", static_cast<int>(ETFClassID::CEconEntity));
	engine->RegisterEnumValue("ETFClassID", "CHandleTest", static_cast<int>(ETFClassID::CHandleTest));
	engine->RegisterEnumValue("ETFClassID", "CTeamplayRoundBasedRulesProxy",
				  static_cast<int>(ETFClassID::CTeamplayRoundBasedRulesProxy));
	engine->RegisterEnumValue("ETFClassID", "CTeamRoundTimer", static_cast<int>(ETFClassID::CTeamRoundTimer));
	engine->RegisterEnumValue("ETFClassID", "CSpriteTrail", static_cast<int>(ETFClassID::CSpriteTrail));
	engine->RegisterEnumValue("ETFClassID", "CSpriteOriented", static_cast<int>(ETFClassID::CSpriteOriented));
	engine->RegisterEnumValue("ETFClassID", "CSprite", static_cast<int>(ETFClassID::CSprite));
	engine->RegisterEnumValue("ETFClassID", "CRagdollPropAttached",
				  static_cast<int>(ETFClassID::CRagdollPropAttached));
	engine->RegisterEnumValue("ETFClassID", "CRagdollProp", static_cast<int>(ETFClassID::CRagdollProp));
	engine->RegisterEnumValue("ETFClassID", "CPoseController", static_cast<int>(ETFClassID::CPoseController));
	engine->RegisterEnumValue("ETFClassID", "CGameRulesProxy", static_cast<int>(ETFClassID::CGameRulesProxy));
	engine->RegisterEnumValue("ETFClassID", "CInfoLadderDismount",
				  static_cast<int>(ETFClassID::CInfoLadderDismount));
	engine->RegisterEnumValue("ETFClassID", "CFuncLadder", static_cast<int>(ETFClassID::CFuncLadder));
	engine->RegisterEnumValue("ETFClassID", "CEnvDetailController",
				  static_cast<int>(ETFClassID::CEnvDetailController));
	engine->RegisterEnumValue("ETFClassID", "CWorld", static_cast<int>(ETFClassID::CWorld));
	engine->RegisterEnumValue("ETFClassID", "CWaterLODControl", static_cast<int>(ETFClassID::CWaterLODControl));
	engine->RegisterEnumValue("ETFClassID", "CWaterBullet", static_cast<int>(ETFClassID::CWaterBullet));
	engine->RegisterEnumValue("ETFClassID", "CVoteController", static_cast<int>(ETFClassID::CVoteController));
	engine->RegisterEnumValue("ETFClassID", "CVGuiScreen", static_cast<int>(ETFClassID::CVGuiScreen));
	engine->RegisterEnumValue("ETFClassID", "CPropJeep", static_cast<int>(ETFClassID::CPropJeep));
	engine->RegisterEnumValue("ETFClassID", "CPropVehicleChoreoGeneric",
				  static_cast<int>(ETFClassID::CPropVehicleChoreoGeneric));
	engine->RegisterEnumValue("ETFClassID", "CTest_ProxyToggle_Networkable",
				  static_cast<int>(ETFClassID::CTest_ProxyToggle_Networkable));
	engine->RegisterEnumValue("ETFClassID", "CTesla", static_cast<int>(ETFClassID::CTesla));
	engine->RegisterEnumValue("ETFClassID", "CTeamTrainWatcher", static_cast<int>(ETFClassID::CTeamTrainWatcher));
	engine->RegisterEnumValue("ETFClassID", "CBaseTeamObjectiveResource",
				  static_cast<int>(ETFClassID::CBaseTeamObjectiveResource));
	engine->RegisterEnumValue("ETFClassID", "CTeam", static_cast<int>(ETFClassID::CTeam));
	engine->RegisterEnumValue("ETFClassID", "CSun", static_cast<int>(ETFClassID::CSun));
	engine->RegisterEnumValue("ETFClassID", "CParticlePerformanceMonitor",
				  static_cast<int>(ETFClassID::CParticlePerformanceMonitor));
	engine->RegisterEnumValue("ETFClassID", "CSpotlightEnd", static_cast<int>(ETFClassID::CSpotlightEnd));
	engine->RegisterEnumValue("ETFClassID", "CSlideshowDisplay", static_cast<int>(ETFClassID::CSlideshowDisplay));
	engine->RegisterEnumValue("ETFClassID", "CShadowControl", static_cast<int>(ETFClassID::CShadowControl));
	engine->RegisterEnumValue("ETFClassID", "CSceneEntity", static_cast<int>(ETFClassID::CSceneEntity));
	engine->RegisterEnumValue("ETFClassID", "CRopeKeyframe", static_cast<int>(ETFClassID::CRopeKeyframe));
	engine->RegisterEnumValue("ETFClassID", "CRagdollManager", static_cast<int>(ETFClassID::CRagdollManager));
	engine->RegisterEnumValue("ETFClassID", "CPhysicsPropMultiplayer",
				  static_cast<int>(ETFClassID::CPhysicsPropMultiplayer));
	engine->RegisterEnumValue("ETFClassID", "CPhysBoxMultiplayer",
				  static_cast<int>(ETFClassID::CPhysBoxMultiplayer));
	engine->RegisterEnumValue("ETFClassID", "CBasePropDoor", static_cast<int>(ETFClassID::CBasePropDoor));
	engine->RegisterEnumValue("ETFClassID", "CDynamicProp", static_cast<int>(ETFClassID::CDynamicProp));
	engine->RegisterEnumValue("ETFClassID", "CPointWorldText", static_cast<int>(ETFClassID::CPointWorldText));
	engine->RegisterEnumValue("ETFClassID", "CPointCommentaryNode",
				  static_cast<int>(ETFClassID::CPointCommentaryNode));
	engine->RegisterEnumValue("ETFClassID", "CPointCamera", static_cast<int>(ETFClassID::CPointCamera));
	engine->RegisterEnumValue("ETFClassID", "CPlayerResource", static_cast<int>(ETFClassID::CPlayerResource));
	engine->RegisterEnumValue("ETFClassID", "CPlasma", static_cast<int>(ETFClassID::CPlasma));
	engine->RegisterEnumValue("ETFClassID", "CPhysMagnet", static_cast<int>(ETFClassID::CPhysMagnet));
	engine->RegisterEnumValue("ETFClassID", "CPhysicsProp", static_cast<int>(ETFClassID::CPhysicsProp));
	engine->RegisterEnumValue("ETFClassID", "CPhysBox", static_cast<int>(ETFClassID::CPhysBox));
	engine->RegisterEnumValue("ETFClassID", "CParticleSystem", static_cast<int>(ETFClassID::CParticleSystem));
	engine->RegisterEnumValue("ETFClassID", "CMaterialModifyControl",
				  static_cast<int>(ETFClassID::CMaterialModifyControl));
	engine->RegisterEnumValue("ETFClassID", "CLightGlow", static_cast<int>(ETFClassID::CLightGlow));
	engine->RegisterEnumValue("ETFClassID", "CInfoOverlayAccessor",
				  static_cast<int>(ETFClassID::CInfoOverlayAccessor));
	engine->RegisterEnumValue("ETFClassID", "CFuncTrackTrain", static_cast<int>(ETFClassID::CFuncTrackTrain));
	engine->RegisterEnumValue("ETFClassID", "CFuncSmokeVolume", static_cast<int>(ETFClassID::CFuncSmokeVolume));
	engine->RegisterEnumValue("ETFClassID", "CFuncRotating", static_cast<int>(ETFClassID::CFuncRotating));
	engine->RegisterEnumValue("ETFClassID", "CFuncReflectiveGlass",
				  static_cast<int>(ETFClassID::CFuncReflectiveGlass));
	engine->RegisterEnumValue("ETFClassID", "CFuncOccluder", static_cast<int>(ETFClassID::CFuncOccluder));
	engine->RegisterEnumValue("ETFClassID", "CFuncMonitor", static_cast<int>(ETFClassID::CFuncMonitor));
	engine->RegisterEnumValue("ETFClassID", "CFunc_LOD", static_cast<int>(ETFClassID::CFunc_LOD));
	engine->RegisterEnumValue("ETFClassID", "CTEDust", static_cast<int>(ETFClassID::CTEDust));
	engine->RegisterEnumValue("ETFClassID", "CFunc_Dust", static_cast<int>(ETFClassID::CFunc_Dust));
	engine->RegisterEnumValue("ETFClassID", "CFuncConveyor", static_cast<int>(ETFClassID::CFuncConveyor));
	engine->RegisterEnumValue("ETFClassID", "CBreakableSurface", static_cast<int>(ETFClassID::CBreakableSurface));
	engine->RegisterEnumValue("ETFClassID", "CFuncAreaPortalWindow",
				  static_cast<int>(ETFClassID::CFuncAreaPortalWindow));
	engine->RegisterEnumValue("ETFClassID", "CFish", static_cast<int>(ETFClassID::CFish));
	engine->RegisterEnumValue("ETFClassID", "CEntityFlame", static_cast<int>(ETFClassID::CEntityFlame));
	engine->RegisterEnumValue("ETFClassID", "CFireSmoke", static_cast<int>(ETFClassID::CFireSmoke));
	engine->RegisterEnumValue("ETFClassID", "CEnvTonemapController",
				  static_cast<int>(ETFClassID::CEnvTonemapController));
	engine->RegisterEnumValue("ETFClassID", "CEnvScreenEffect", static_cast<int>(ETFClassID::CEnvScreenEffect));
	engine->RegisterEnumValue("ETFClassID", "CEnvScreenOverlay", static_cast<int>(ETFClassID::CEnvScreenOverlay));
	engine->RegisterEnumValue("ETFClassID", "CEnvProjectedTexture",
				  static_cast<int>(ETFClassID::CEnvProjectedTexture));
	engine->RegisterEnumValue("ETFClassID", "CEnvParticleScript", static_cast<int>(ETFClassID::CEnvParticleScript));
	engine->RegisterEnumValue("ETFClassID", "CFogController", static_cast<int>(ETFClassID::CFogController));
	engine->RegisterEnumValue("ETFClassID", "CEntityParticleTrail",
				  static_cast<int>(ETFClassID::CEntityParticleTrail));
	engine->RegisterEnumValue("ETFClassID", "CEntityDissolve", static_cast<int>(ETFClassID::CEntityDissolve));
	engine->RegisterEnumValue("ETFClassID", "CDynamicLight", static_cast<int>(ETFClassID::CDynamicLight));
	engine->RegisterEnumValue("ETFClassID", "CColorCorrectionVolume",
				  static_cast<int>(ETFClassID::CColorCorrectionVolume));
	engine->RegisterEnumValue("ETFClassID", "CColorCorrection", static_cast<int>(ETFClassID::CColorCorrection));
	engine->RegisterEnumValue("ETFClassID", "CBreakableProp", static_cast<int>(ETFClassID::CBreakableProp));
	engine->RegisterEnumValue("ETFClassID", "CBasePlayer", static_cast<int>(ETFClassID::CBasePlayer));
	engine->RegisterEnumValue("ETFClassID", "CBaseFlex", static_cast<int>(ETFClassID::CBaseFlex));
	engine->RegisterEnumValue("ETFClassID", "CBaseEntity", static_cast<int>(ETFClassID::CBaseEntity));
	engine->RegisterEnumValue("ETFClassID", "CBaseDoor", static_cast<int>(ETFClassID::CBaseDoor));
	engine->RegisterEnumValue("ETFClassID", "CBaseCombatCharacter",
				  static_cast<int>(ETFClassID::CBaseCombatCharacter));
	engine->RegisterEnumValue("ETFClassID", "CBaseAnimatingOverlay",
				  static_cast<int>(ETFClassID::CBaseAnimatingOverlay));
	engine->RegisterEnumValue("ETFClassID", "CBoneFollower", static_cast<int>(ETFClassID::CBoneFollower));
	engine->RegisterEnumValue("ETFClassID", "CBaseAnimating", static_cast<int>(ETFClassID::CBaseAnimating));
	engine->RegisterEnumValue("ETFClassID", "CInfoLightingRelative",
				  static_cast<int>(ETFClassID::CInfoLightingRelative));
	engine->RegisterEnumValue("ETFClassID", "CAI_BaseNPC", static_cast<int>(ETFClassID::CAI_BaseNPC));
	engine->RegisterEnumValue("ETFClassID", "CBeam", static_cast<int>(ETFClassID::CBeam));
	engine->RegisterEnumValue("ETFClassID", "CBaseViewModel", static_cast<int>(ETFClassID::CBaseViewModel));
	engine->RegisterEnumValue("ETFClassID", "CBaseProjectile", static_cast<int>(ETFClassID::CBaseProjectile));
	engine->RegisterEnumValue("ETFClassID", "CBaseParticleEntity",
				  static_cast<int>(ETFClassID::CBaseParticleEntity));
	engine->RegisterEnumValue("ETFClassID", "CBaseGrenade", static_cast<int>(ETFClassID::CBaseGrenade));
	engine->RegisterEnumValue("ETFClassID", "CBaseCombatWeapon", static_cast<int>(ETFClassID::CBaseCombatWeapon));

	// draw flags
	engine->RegisterEnum("EDrawModelFlags");
	engine->RegisterEnumValue("EDrawModelFlags", "NONE", 0x00000000);
	engine->RegisterEnumValue("EDrawModelFlags", "RENDER", 0x00000001);
	engine->RegisterEnumValue("EDrawModelFlags", "VIEWXFORMATTACHMENTS", 0x00000002);
	engine->RegisterEnumValue("EDrawModelFlags", "DRAWTRANSLUCENTSUBMODELS", 0x00000004);
	engine->RegisterEnumValue("EDrawModelFlags", "TWOPASS", 0x00000008);
	engine->RegisterEnumValue("EDrawModelFlags", "STATIC_LIGHTING", 0x00000010);
	engine->RegisterEnumValue("EDrawModelFlags", "WIREFRAME", 0x00000020);
	engine->RegisterEnumValue("EDrawModelFlags", "ITEM_BLINK", 0x00000040);
	engine->RegisterEnumValue("EDrawModelFlags", "NOSHADOWS", 0x00000080);
	engine->RegisterEnumValue("EDrawModelFlags", "WIREFRAME_VCOLLIDE", 0x00000100);
	engine->RegisterEnumValue("EDrawModelFlags", "NO_OVERRIDE_FOR_ATTACH", 0x00000200);
	engine->RegisterEnumValue("EDrawModelFlags", "GENERATE_STATS", 0x01000000);
	engine->RegisterEnumValue("EDrawModelFlags", "SSAODEPTHTEXTURE", 0x08000000);
	engine->RegisterEnumValue("EDrawModelFlags", "SHADOWDEPTHTEXTURE", 0x40000000);
	engine->RegisterEnumValue("EDrawModelFlags", "TRANSPARENCY", 0x80000000);

	// input (oh no)
	engine->RegisterEnum("EButtonCode");
	engine->RegisterEnumValue("EButtonCode", "BUTTON_CODE_INVALID", ButtonCode_t::BUTTON_CODE_INVALID);
	engine->RegisterEnumValue("EButtonCode", "BUTTON_CODE_NONE", ButtonCode_t::BUTTON_CODE_NONE);
	engine->RegisterEnumValue("EButtonCode", "KEY_FIRST", ButtonCode_t::KEY_FIRST);
	engine->RegisterEnumValue("EButtonCode", "KEY_NONE", ButtonCode_t::KEY_NONE);
	engine->RegisterEnumValue("EButtonCode", "KEY_0", ButtonCode_t::KEY_0);
	engine->RegisterEnumValue("EButtonCode", "KEY_1", ButtonCode_t::KEY_1);
	engine->RegisterEnumValue("EButtonCode", "KEY_2", ButtonCode_t::KEY_2);
	engine->RegisterEnumValue("EButtonCode", "KEY_3", ButtonCode_t::KEY_3);
	engine->RegisterEnumValue("EButtonCode", "KEY_4", ButtonCode_t::KEY_4);
	engine->RegisterEnumValue("EButtonCode", "KEY_5", ButtonCode_t::KEY_5);
	engine->RegisterEnumValue("EButtonCode", "KEY_6", ButtonCode_t::KEY_6);
	engine->RegisterEnumValue("EButtonCode", "KEY_7", ButtonCode_t::KEY_7);
	engine->RegisterEnumValue("EButtonCode", "KEY_8", ButtonCode_t::KEY_8);
	engine->RegisterEnumValue("EButtonCode", "KEY_9", ButtonCode_t::KEY_9);
	engine->RegisterEnumValue("EButtonCode", "KEY_A", ButtonCode_t::KEY_A);
	engine->RegisterEnumValue("EButtonCode", "KEY_B", ButtonCode_t::KEY_B);
	engine->RegisterEnumValue("EButtonCode", "KEY_C", ButtonCode_t::KEY_C);
	engine->RegisterEnumValue("EButtonCode", "KEY_D", ButtonCode_t::KEY_D);
	engine->RegisterEnumValue("EButtonCode", "KEY_E", ButtonCode_t::KEY_E);
	engine->RegisterEnumValue("EButtonCode", "KEY_F", ButtonCode_t::KEY_F);
	engine->RegisterEnumValue("EButtonCode", "KEY_G", ButtonCode_t::KEY_G);
	engine->RegisterEnumValue("EButtonCode", "KEY_H", ButtonCode_t::KEY_H);
	engine->RegisterEnumValue("EButtonCode", "KEY_I", ButtonCode_t::KEY_I);
	engine->RegisterEnumValue("EButtonCode", "KEY_J", ButtonCode_t::KEY_J);
	engine->RegisterEnumValue("EButtonCode", "KEY_K", ButtonCode_t::KEY_K);
	engine->RegisterEnumValue("EButtonCode", "KEY_L", ButtonCode_t::KEY_L);
	engine->RegisterEnumValue("EButtonCode", "KEY_M", ButtonCode_t::KEY_M);
	engine->RegisterEnumValue("EButtonCode", "KEY_N", ButtonCode_t::KEY_N);
	engine->RegisterEnumValue("EButtonCode", "KEY_O", ButtonCode_t::KEY_O);
	engine->RegisterEnumValue("EButtonCode", "KEY_P", ButtonCode_t::KEY_P);
	engine->RegisterEnumValue("EButtonCode", "KEY_Q", ButtonCode_t::KEY_Q);
	engine->RegisterEnumValue("EButtonCode", "KEY_R", ButtonCode_t::KEY_R);
	engine->RegisterEnumValue("EButtonCode", "KEY_S", ButtonCode_t::KEY_S);
	engine->RegisterEnumValue("EButtonCode", "KEY_T", ButtonCode_t::KEY_T);
	engine->RegisterEnumValue("EButtonCode", "KEY_U", ButtonCode_t::KEY_U);
	engine->RegisterEnumValue("EButtonCode", "KEY_V", ButtonCode_t::KEY_V);
	engine->RegisterEnumValue("EButtonCode", "KEY_W", ButtonCode_t::KEY_W);
	engine->RegisterEnumValue("EButtonCode", "KEY_X", ButtonCode_t::KEY_X);
	engine->RegisterEnumValue("EButtonCode", "KEY_Y", ButtonCode_t::KEY_Y);
	engine->RegisterEnumValue("EButtonCode", "KEY_Z", ButtonCode_t::KEY_Z);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_0", ButtonCode_t::KEY_PAD_0);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_1", ButtonCode_t::KEY_PAD_1);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_2", ButtonCode_t::KEY_PAD_2);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_3", ButtonCode_t::KEY_PAD_3);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_4", ButtonCode_t::KEY_PAD_4);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_5", ButtonCode_t::KEY_PAD_5);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_6", ButtonCode_t::KEY_PAD_6);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_7", ButtonCode_t::KEY_PAD_7);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_8", ButtonCode_t::KEY_PAD_8);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_9", ButtonCode_t::KEY_PAD_9);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_DIVIDE", ButtonCode_t::KEY_PAD_DIVIDE);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_MULTIPLY", ButtonCode_t::KEY_PAD_MULTIPLY);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_MINUS", ButtonCode_t::KEY_PAD_MINUS);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_PLUS", ButtonCode_t::KEY_PAD_PLUS);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_ENTER", ButtonCode_t::KEY_PAD_ENTER);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAD_DECIMAL", ButtonCode_t::KEY_PAD_DECIMAL);
	engine->RegisterEnumValue("EButtonCode", "KEY_LBRACKET", ButtonCode_t::KEY_LBRACKET);
	engine->RegisterEnumValue("EButtonCode", "KEY_RBRACKET", ButtonCode_t::KEY_RBRACKET);
	engine->RegisterEnumValue("EButtonCode", "KEY_SEMICOLON", ButtonCode_t::KEY_SEMICOLON);
	engine->RegisterEnumValue("EButtonCode", "KEY_APOSTROPHE", ButtonCode_t::KEY_APOSTROPHE);
	engine->RegisterEnumValue("EButtonCode", "KEY_BACKQUOTE", ButtonCode_t::KEY_BACKQUOTE);
	engine->RegisterEnumValue("EButtonCode", "KEY_COMMA", ButtonCode_t::KEY_COMMA);
	engine->RegisterEnumValue("EButtonCode", "KEY_PERIOD", ButtonCode_t::KEY_PERIOD);
	engine->RegisterEnumValue("EButtonCode", "KEY_SLASH", ButtonCode_t::KEY_SLASH);
	engine->RegisterEnumValue("EButtonCode", "KEY_BACKSLASH", ButtonCode_t::KEY_BACKSLASH);
	engine->RegisterEnumValue("EButtonCode", "KEY_MINUS", ButtonCode_t::KEY_MINUS);
	engine->RegisterEnumValue("EButtonCode", "KEY_EQUAL", ButtonCode_t::KEY_EQUAL);
	engine->RegisterEnumValue("EButtonCode", "KEY_ENTER", ButtonCode_t::KEY_ENTER);
	engine->RegisterEnumValue("EButtonCode", "KEY_SPACE", ButtonCode_t::KEY_SPACE);
	engine->RegisterEnumValue("EButtonCode", "KEY_BACKSPACE", ButtonCode_t::KEY_BACKSPACE);
	engine->RegisterEnumValue("EButtonCode", "KEY_TAB", ButtonCode_t::KEY_TAB);
	engine->RegisterEnumValue("EButtonCode", "KEY_CAPSLOCK", ButtonCode_t::KEY_CAPSLOCK);
	engine->RegisterEnumValue("EButtonCode", "KEY_NUMLOCK", ButtonCode_t::KEY_NUMLOCK);
	engine->RegisterEnumValue("EButtonCode", "KEY_ESCAPE", ButtonCode_t::KEY_ESCAPE);
	engine->RegisterEnumValue("EButtonCode", "KEY_SCROLLLOCK", ButtonCode_t::KEY_SCROLLLOCK);
	engine->RegisterEnumValue("EButtonCode", "KEY_INSERT", ButtonCode_t::KEY_INSERT);
	engine->RegisterEnumValue("EButtonCode", "KEY_DELETE", ButtonCode_t::KEY_DELETE);
	engine->RegisterEnumValue("EButtonCode", "KEY_HOME", ButtonCode_t::KEY_HOME);
	engine->RegisterEnumValue("EButtonCode", "KEY_END", ButtonCode_t::KEY_END);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAGEUP", ButtonCode_t::KEY_PAGEUP);
	engine->RegisterEnumValue("EButtonCode", "KEY_PAGEDOWN", ButtonCode_t::KEY_PAGEDOWN);
	engine->RegisterEnumValue("EButtonCode", "KEY_BREAK", ButtonCode_t::KEY_BREAK);
	engine->RegisterEnumValue("EButtonCode", "KEY_LSHIFT", ButtonCode_t::KEY_LSHIFT);
	engine->RegisterEnumValue("EButtonCode", "KEY_RSHIFT", ButtonCode_t::KEY_RSHIFT);
	engine->RegisterEnumValue("EButtonCode", "KEY_LALT", ButtonCode_t::KEY_LALT);
	engine->RegisterEnumValue("EButtonCode", "KEY_RALT", ButtonCode_t::KEY_RALT);
	engine->RegisterEnumValue("EButtonCode", "KEY_LCONTROL", ButtonCode_t::KEY_LCONTROL);
	engine->RegisterEnumValue("EButtonCode", "KEY_RCONTROL", ButtonCode_t::KEY_RCONTROL);
	engine->RegisterEnumValue("EButtonCode", "KEY_LWIN", ButtonCode_t::KEY_LWIN);
	engine->RegisterEnumValue("EButtonCode", "KEY_RWIN", ButtonCode_t::KEY_RWIN);
	engine->RegisterEnumValue("EButtonCode", "KEY_APP", ButtonCode_t::KEY_APP);
	engine->RegisterEnumValue("EButtonCode", "KEY_UP", ButtonCode_t::KEY_UP);
	engine->RegisterEnumValue("EButtonCode", "KEY_LEFT", ButtonCode_t::KEY_LEFT);
	engine->RegisterEnumValue("EButtonCode", "KEY_DOWN", ButtonCode_t::KEY_DOWN);
	engine->RegisterEnumValue("EButtonCode", "KEY_RIGHT", ButtonCode_t::KEY_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "KEY_F1", ButtonCode_t::KEY_F1);
	engine->RegisterEnumValue("EButtonCode", "KEY_F2", ButtonCode_t::KEY_F2);
	engine->RegisterEnumValue("EButtonCode", "KEY_F3", ButtonCode_t::KEY_F3);
	engine->RegisterEnumValue("EButtonCode", "KEY_F4", ButtonCode_t::KEY_F4);
	engine->RegisterEnumValue("EButtonCode", "KEY_F5", ButtonCode_t::KEY_F5);
	engine->RegisterEnumValue("EButtonCode", "KEY_F6", ButtonCode_t::KEY_F6);
	engine->RegisterEnumValue("EButtonCode", "KEY_F7", ButtonCode_t::KEY_F7);
	engine->RegisterEnumValue("EButtonCode", "KEY_F8", ButtonCode_t::KEY_F8);
	engine->RegisterEnumValue("EButtonCode", "KEY_F9", ButtonCode_t::KEY_F9);
	engine->RegisterEnumValue("EButtonCode", "KEY_F10", ButtonCode_t::KEY_F10);
	engine->RegisterEnumValue("EButtonCode", "KEY_F11", ButtonCode_t::KEY_F11);
	engine->RegisterEnumValue("EButtonCode", "KEY_F12", ButtonCode_t::KEY_F12);
	engine->RegisterEnumValue("EButtonCode", "KEY_CAPSLOCKTOGGLE", ButtonCode_t::KEY_CAPSLOCKTOGGLE);
	engine->RegisterEnumValue("EButtonCode", "KEY_NUMLOCKTOGGLE", ButtonCode_t::KEY_NUMLOCKTOGGLE);
	engine->RegisterEnumValue("EButtonCode", "KEY_SCROLLLOCKTOGGLE", ButtonCode_t::KEY_SCROLLLOCKTOGGLE);
	engine->RegisterEnumValue("EButtonCode", "KEY_LAST", ButtonCode_t::KEY_LAST);
	engine->RegisterEnumValue("EButtonCode", "KEY_COUNT", ButtonCode_t::KEY_COUNT);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_FIRST", ButtonCode_t::MOUSE_FIRST);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_LEFT", ButtonCode_t::MOUSE_LEFT);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_RIGHT", ButtonCode_t::MOUSE_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_MIDDLE", ButtonCode_t::MOUSE_MIDDLE);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_4", ButtonCode_t::MOUSE_4);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_5", ButtonCode_t::MOUSE_5);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_WHEEL_UP", ButtonCode_t::MOUSE_WHEEL_UP);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_WHEEL_DOWN", ButtonCode_t::MOUSE_WHEEL_DOWN);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_LAST", ButtonCode_t::MOUSE_LAST);
	engine->RegisterEnumValue("EButtonCode", "MOUSE_COUNT", ButtonCode_t::MOUSE_COUNT);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_FIRST", ButtonCode_t::JOYSTICK_FIRST);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_FIRST_BUTTON", ButtonCode_t::JOYSTICK_FIRST_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_LAST_BUTTON", ButtonCode_t::JOYSTICK_LAST_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_FIRST_POV_BUTTON", ButtonCode_t::JOYSTICK_FIRST_POV_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_LAST_POV_BUTTON", ButtonCode_t::JOYSTICK_LAST_POV_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_FIRST_AXIS_BUTTON",
				  ButtonCode_t::JOYSTICK_FIRST_AXIS_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_LAST_AXIS_BUTTON", ButtonCode_t::JOYSTICK_LAST_AXIS_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "JOYSTICK_LAST", ButtonCode_t::JOYSTICK_LAST);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_FIRST", ButtonCode_t::NOVINT_FIRST);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_LOGO_0", ButtonCode_t::NOVINT_LOGO_0);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_TRIANGLE_0", ButtonCode_t::NOVINT_TRIANGLE_0);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_BOLT_0", ButtonCode_t::NOVINT_BOLT_0);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_PLUS_0", ButtonCode_t::NOVINT_PLUS_0);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_LOGO_1", ButtonCode_t::NOVINT_LOGO_1);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_TRIANGLE_1", ButtonCode_t::NOVINT_TRIANGLE_1);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_BOLT_1", ButtonCode_t::NOVINT_BOLT_1);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_PLUS_1", ButtonCode_t::NOVINT_PLUS_1);
	engine->RegisterEnumValue("EButtonCode", "NOVINT_LAST", ButtonCode_t::NOVINT_LAST);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_FIRST", ButtonCode_t::STEAMCONTROLLER_FIRST);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_FIRST_BUTTON",
				  ButtonCode_t::STEAMCONTROLLER_FIRST_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LAST_BUTTON",
				  ButtonCode_t::STEAMCONTROLLER_LAST_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_FIRST_AXIS_BUTTON",
				  ButtonCode_t::STEAMCONTROLLER_FIRST_AXIS_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LAST_AXIS_BUTTON",
				  ButtonCode_t::STEAMCONTROLLER_LAST_AXIS_BUTTON);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LAST", ButtonCode_t::STEAMCONTROLLER_LAST);
	engine->RegisterEnumValue("EButtonCode", "BUTTON_CODE_LAST", ButtonCode_t::BUTTON_CODE_LAST);
	engine->RegisterEnumValue("EButtonCode", "BUTTON_CODE_COUNT", ButtonCode_t::BUTTON_CODE_COUNT);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_UP", ButtonCode_t::KEY_XBUTTON_UP);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_RIGHT", ButtonCode_t::KEY_XBUTTON_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_DOWN", ButtonCode_t::KEY_XBUTTON_DOWN);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_LEFT", ButtonCode_t::KEY_XBUTTON_LEFT);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_A", ButtonCode_t::KEY_XBUTTON_A);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_B", ButtonCode_t::KEY_XBUTTON_B);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_X", ButtonCode_t::KEY_XBUTTON_X);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_Y", ButtonCode_t::KEY_XBUTTON_Y);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_LEFT_SHOULDER", ButtonCode_t::KEY_XBUTTON_LEFT_SHOULDER);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_RIGHT_SHOULDER",
				  ButtonCode_t::KEY_XBUTTON_RIGHT_SHOULDER);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_BACK", ButtonCode_t::KEY_XBUTTON_BACK);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_START", ButtonCode_t::KEY_XBUTTON_START);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_STICK1", ButtonCode_t::KEY_XBUTTON_STICK1);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_STICK2", ButtonCode_t::KEY_XBUTTON_STICK2);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK1_RIGHT", ButtonCode_t::KEY_XSTICK1_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK1_LEFT", ButtonCode_t::KEY_XSTICK1_LEFT);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK1_DOWN", ButtonCode_t::KEY_XSTICK1_DOWN);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK1_UP", ButtonCode_t::KEY_XSTICK1_UP);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_LTRIGGER", ButtonCode_t::KEY_XBUTTON_LTRIGGER);
	engine->RegisterEnumValue("EButtonCode", "KEY_XBUTTON_RTRIGGER", ButtonCode_t::KEY_XBUTTON_RTRIGGER);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK2_RIGHT", ButtonCode_t::KEY_XSTICK2_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK2_LEFT", ButtonCode_t::KEY_XSTICK2_LEFT);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK2_DOWN", ButtonCode_t::KEY_XSTICK2_DOWN);
	engine->RegisterEnumValue("EButtonCode", "KEY_XSTICK2_UP", ButtonCode_t::KEY_XSTICK2_UP);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_A", ButtonCode_t::STEAMCONTROLLER_A);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_B", ButtonCode_t::STEAMCONTROLLER_B);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_X", ButtonCode_t::STEAMCONTROLLER_X);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_Y", ButtonCode_t::STEAMCONTROLLER_Y);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_DPAD_UP", ButtonCode_t::STEAMCONTROLLER_DPAD_UP);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_DPAD_RIGHT",
				  ButtonCode_t::STEAMCONTROLLER_DPAD_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_DPAD_DOWN", ButtonCode_t::STEAMCONTROLLER_DPAD_DOWN);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_DPAD_LEFT", ButtonCode_t::STEAMCONTROLLER_DPAD_LEFT);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_BUMPER",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_BUMPER);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_BUMPER",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_BUMPER);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_TRIGGER",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_TRIGGER);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_TRIGGER",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_TRIGGER);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_GRIP", ButtonCode_t::STEAMCONTROLLER_LEFT_GRIP);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_GRIP",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_GRIP);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_PAD_FINGERDOWN",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_PAD_FINGERDOWN);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_PAD_FINGERDOWN",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_PAD_FINGERDOWN);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_PAD_CLICK",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_PAD_CLICK);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_PAD_CLICK",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_PAD_CLICK);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_PAD_UP",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_PAD_UP);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_PAD_RIGHT",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_PAD_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_PAD_DOWN",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_PAD_DOWN);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_LEFT_PAD_LEFT",
				  ButtonCode_t::STEAMCONTROLLER_LEFT_PAD_LEFT);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_PAD_UP",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_PAD_UP);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_PAD_RIGHT",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_PAD_RIGHT);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_PAD_DOWN",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_PAD_DOWN);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_RIGHT_PAD_LEFT",
				  ButtonCode_t::STEAMCONTROLLER_RIGHT_PAD_LEFT);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_SELECT", ButtonCode_t::STEAMCONTROLLER_SELECT);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_START", ButtonCode_t::STEAMCONTROLLER_START);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_STEAM", ButtonCode_t::STEAMCONTROLLER_STEAM);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_INACTIVE_START",
				  ButtonCode_t::STEAMCONTROLLER_INACTIVE_START);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F1", ButtonCode_t::STEAMCONTROLLER_F1);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F2", ButtonCode_t::STEAMCONTROLLER_F2);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F3", ButtonCode_t::STEAMCONTROLLER_F3);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F4", ButtonCode_t::STEAMCONTROLLER_F4);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F5", ButtonCode_t::STEAMCONTROLLER_F5);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F6", ButtonCode_t::STEAMCONTROLLER_F6);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F7", ButtonCode_t::STEAMCONTROLLER_F7);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F8", ButtonCode_t::STEAMCONTROLLER_F8);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F9", ButtonCode_t::STEAMCONTROLLER_F9);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F10", ButtonCode_t::STEAMCONTROLLER_F10);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F11", ButtonCode_t::STEAMCONTROLLER_F11);
	engine->RegisterEnumValue("EButtonCode", "STEAMCONTROLLER_F12", ButtonCode_t::STEAMCONTROLLER_F12);
}