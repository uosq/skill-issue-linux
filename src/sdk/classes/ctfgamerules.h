#pragma once

#include "../netvars/netvar.h"

class CTeamplayRules
{
public:
};

class CTeamplayRoundBasedRules : public CTeamplayRules
{
public:
	NETVAR(m_iRoundState, "CTeamplayRoundBasedRulesProxy->m_iRoundState", int);
	NETVAR(m_bInOvertime, "CTeamplayRoundBasedRulesProxy->m_bInOvertime", bool);
	NETVAR(m_bInSetup, "CTeamplayRoundBasedRulesProxy->m_bInSetup", bool);
	NETVAR(m_bSwitchedTeamsThisRound, "CTeamplayRoundBasedRulesProxy->m_bSwitchedTeamsThisRound", bool);
	NETVAR(m_iWinningTeam, "CTeamplayRoundBasedRulesProxy->m_iWinningTeam", int);
	NETVAR(m_iWinReason, "CTeamplayRoundBasedRulesProxy->m_iWinReason", int);
	NETVAR(m_bInWaitingForPlayers, "CTeamplayRoundBasedRulesProxy->m_bInWaitingForPlayers", bool);
	NETVAR(m_bAwaitingReadyRestart, "CTeamplayRoundBasedRulesProxy->m_bAwaitingReadyRestart", bool);
	NETVAR(m_flRestartRoundTime, "CTeamplayRoundBasedRulesProxy->m_flRestartRoundTime", float);
	NETVAR(m_flMapResetTime, "CTeamplayRoundBasedRulesProxy->m_flMapResetTime", float);
	NETVAR(m_flNextRespawnWave, "CTeamplayRoundBasedRulesProxy->m_flNextRespawnWave", void*);
	NETVAR(m_bTeamReady, "CTeamplayRoundBasedRulesProxy->m_bTeamReady", void*);
	NETVAR(m_bStopWatch, "CTeamplayRoundBasedRulesProxy->m_bStopWatch", bool);
	NETVAR(m_bMultipleTrains, "CTeamplayRoundBasedRulesProxy->m_bMultipleTrains", bool);
	NETVAR(m_bPlayerReady, "CTeamplayRoundBasedRulesProxy->m_bPlayerReady", void*);
	NETVAR(m_bCheatsEnabledDuringLevel, "CTeamplayRoundBasedRulesProxy->m_bCheatsEnabledDuringLevel", bool);
	NETVAR(m_nRoundsPlayed, "CTeamplayRoundBasedRulesProxy->m_nRoundsPlayed", int);
	NETVAR(m_flCountdownTime, "CTeamplayRoundBasedRulesProxy->m_flCountdownTime", float);
	NETVAR(m_flStateTransitionTime, "CTeamplayRoundBasedRulesProxy->m_flStateTransitionTime", float);
	NETVAR(m_TeamRespawnWaveTimes, "CTeamplayRoundBasedRulesProxy->m_TeamRespawnWaveTimes", void*);
};

class CTFGameRules : public CTeamplayRoundBasedRules
{
public:
	NETVAR(m_nGameType, "CTFGameRulesProxy->m_nGameType", int);
	NETVAR(m_nStopWatchState, "CTFGameRulesProxy->m_nStopWatchState", int);
	NETVAR(m_pszTeamGoalStringRed, "CTFGameRulesProxy->m_pszTeamGoalStringRed", const char*);
	NETVAR(m_pszTeamGoalStringBlue, "CTFGameRulesProxy->m_pszTeamGoalStringBlue", const char*);
	NETVAR(m_flCapturePointEnableTime, "CTFGameRulesProxy->m_flCapturePointEnableTime", float);
	NETVAR(m_iGlobalAttributeCacheVersion, "CTFGameRulesProxy->m_iGlobalAttributeCacheVersion", int);
	NETVAR(m_nHudType, "CTFGameRulesProxy->m_nHudType", int);
	NETVAR(m_bIsInTraining, "CTFGameRulesProxy->m_bIsInTraining", bool);
	NETVAR(m_bAllowTrainingAchievements, "CTFGameRulesProxy->m_bAllowTrainingAchievements", bool);
	NETVAR(m_bIsWaitingForTrainingContinue, "CTFGameRulesProxy->m_bIsWaitingForTrainingContinue", bool);
	NETVAR(m_bIsTrainingHUDVisible, "CTFGameRulesProxy->m_bIsTrainingHUDVisible", bool);
	NETVAR(m_bIsInItemTestingMode, "CTFGameRulesProxy->m_bIsInItemTestingMode", bool);
	NETVAR(m_hBonusLogic, "CTFGameRulesProxy->m_hBonusLogic", EHANDLE);
	NETVAR(m_bPlayingKoth, "CTFGameRulesProxy->m_bPlayingKoth", bool);
	NETVAR(m_bPowerupMode, "CTFGameRulesProxy->m_bPowerupMode", bool);
	NETVAR(m_bPlayingRobotDestructionMode, "CTFGameRulesProxy->m_bPlayingRobotDestructionMode", bool);
	NETVAR(m_bPlayingMedieval, "CTFGameRulesProxy->m_bPlayingMedieval", bool);
	NETVAR(m_bPlayingHybrid_CTF_CP, "CTFGameRulesProxy->m_bPlayingHybrid_CTF_CP", bool);
	NETVAR(m_bPlayingSpecialDeliveryMode, "CTFGameRulesProxy->m_bPlayingSpecialDeliveryMode", bool);
	NETVAR(m_bPlayingMannVsMachine, "CTFGameRulesProxy->m_bPlayingMannVsMachine", bool);
	NETVAR(m_bMannVsMachineAlarmStatus, "CTFGameRulesProxy->m_bMannVsMachineAlarmStatus", bool);
	NETVAR(m_bHaveMinPlayersToEnableReady, "CTFGameRulesProxy->m_bHaveMinPlayersToEnableReady", bool);
	NETVAR(m_bBountyModeEnabled, "CTFGameRulesProxy->m_bBountyModeEnabled", bool);
	NETVAR(m_bCompetitiveMode, "CTFGameRulesProxy->m_bCompetitiveMode", bool);
	NETVAR(m_nMatchGroupType, "CTFGameRulesProxy->m_nMatchGroupType", int);
	NETVAR(m_bMatchEnded, "CTFGameRulesProxy->m_bMatchEnded", bool);
	NETVAR(m_bHelltowerPlayersInHell, "CTFGameRulesProxy->m_bHelltowerPlayersInHell", bool);
	NETVAR(m_bIsUsingSpells, "CTFGameRulesProxy->m_bIsUsingSpells", bool);
	NETVAR(m_bTruceActive, "CTFGameRulesProxy->m_bTruceActive", bool);
	NETVAR(m_bTeamsSwitched, "CTFGameRulesProxy->m_bTeamsSwitched", bool);
	NETVAR(m_hRedKothTimer, "CTFGameRulesProxy->m_hRedKothTimer", EHANDLE);
	NETVAR(m_hBlueKothTimer, "CTFGameRulesProxy->m_hBlueKothTimer", EHANDLE);
	NETVAR(m_nMapHolidayType, "CTFGameRulesProxy->m_nMapHolidayType", int);
	NETVAR(m_pszCustomUpgradesFile, "CTFGameRulesProxy->m_pszCustomUpgradesFile", const char*);
	NETVAR(m_bShowMatchSummary, "CTFGameRulesProxy->m_bShowMatchSummary", bool);
	NETVAR(m_bMapHasMatchSummaryStage, "CTFGameRulesProxy->m_bMapHasMatchSummaryStage", bool);
	NETVAR(m_bPlayersAreOnMatchSummaryStage, "CTFGameRulesProxy->m_bPlayersAreOnMatchSummaryStage", bool);
	NETVAR(m_bStopWatchWinner, "CTFGameRulesProxy->m_bStopWatchWinner", bool);
	NETVAR(m_ePlayerWantsRematch, "CTFGameRulesProxy->m_ePlayerWantsRematch", void*);
	NETVAR(m_eRematchState, "CTFGameRulesProxy->m_eRematchState", int);
	NETVAR(m_nNextMapVoteOptions, "CTFGameRulesProxy->m_nNextMapVoteOptions", void*);
	NETVAR(m_nBossHealth, "CTFGameRulesProxy->m_nBossHealth", int);
	NETVAR(m_nMaxBossHealth, "CTFGameRulesProxy->m_nMaxBossHealth", int);
	NETVAR(m_fBossNormalizedTravelDistance, "CTFGameRulesProxy->m_fBossNormalizedTravelDistance", int);
	NETVAR(m_itHandle, "CTFGameRulesProxy->m_itHandle", int);
	NETVAR(m_hBirthdayPlayer, "CTFGameRulesProxy->m_hBirthdayPlayer", int);
	NETVAR(m_nHalloweenEffect, "CTFGameRulesProxy->m_nHalloweenEffect", int);
	NETVAR(m_fHalloweenEffectStartTime, "CTFGameRulesProxy->m_fHalloweenEffectStartTime", float);
	NETVAR(m_fHalloweenEffectDuration, "CTFGameRulesProxy->m_fHalloweenEffectDuration", float);
	NETVAR(m_halloweenScenario, "CTFGameRulesProxy->m_halloweenScenario", int);
	NETVAR(m_nForceUpgrades, "CTFGameRulesProxy->m_nForceUpgrades", int);
	NETVAR(m_nForceEscortPushLogic, "CTFGameRulesProxy->m_nForceEscortPushLogic", int);
	NETVAR(m_bRopesHolidayLightsAllowed, "CTFGameRulesProxy->m_bRopesHolidayLightsAllowed", bool);
};

static CTFGameRules* TFGameRules()
{
	static uintptr_t leaInstr = reinterpret_cast<uintptr_t>(sigscan_module("client.so", "48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 ? 31 C0"));
	static uintptr_t g_pGameRules_addr = vtable::ResolveRIP(leaInstr, 3, 7);
	static CTFGameRules* g_pGameRules = *reinterpret_cast<CTFGameRules**>(g_pGameRules_addr);
	return g_pGameRules;
}