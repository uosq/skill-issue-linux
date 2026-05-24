#include "../../../sol3/sol.hpp"

#include "../../../sdk/classes/ctfgamerules.h"
#include "../../../sdk/classes/player.h"

void BindGameRules(sol::state& lua)
{
	auto gamerules = lua.create_named_table("gamerules");

	// CTeamplayRoundBasedRules
	gamerules["get_round_state"] = []() -> int { return TFGameRules() ? TFGameRules()->m_iRoundState() : 0; };
	gamerules["is_in_overtime"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bInOvertime() : false; };
	gamerules["is_in_setup"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bInSetup() : false; };
	gamerules["switched_teams_this_round"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bSwitchedTeamsThisRound() : false; };
	gamerules["get_winning_team"] = []() -> int { return TFGameRules() ? TFGameRules()->m_iWinningTeam() : 0; };
	gamerules["get_win_reason"] = []() -> int { return TFGameRules() ? TFGameRules()->m_iWinReason() : 0; };
	gamerules["is_waiting_for_players"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bInWaitingForPlayers() : false; };
	gamerules["is_awaiting_ready_restart"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bAwaitingReadyRestart() : false; };
	gamerules["get_restart_round_time"] = []() -> float { return TFGameRules() ? TFGameRules()->m_flRestartRoundTime() : 0.0f; };
	gamerules["get_map_reset_time"] = []() -> float { return TFGameRules() ? TFGameRules()->m_flMapResetTime() : 0.0f; };
	gamerules["get_next_respawn_wave"] = []() -> float { return TFGameRules() ? TFGameRules()->m_flNextRespawnWave() : 0.0f; };
	gamerules["is_team_ready"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bTeamReady() : false; };
	gamerules["is_stopwatch"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bStopWatch() : false; };
	gamerules["has_multiple_trains"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bMultipleTrains() : false; };
	gamerules["is_player_ready"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayerReady() : false; };
	gamerules["cheats_enabled_during_level"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bCheatsEnabledDuringLevel() : false; };
	gamerules["get_rounds_played"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nRoundsPlayed() : 0; };
	gamerules["get_countdown_time"] = []() -> float { return TFGameRules() ? TFGameRules()->m_flCountdownTime() : 0.0f; };
	gamerules["get_state_transition_time"] = []() -> float { return TFGameRules() ? TFGameRules()->m_flStateTransitionTime() : 0.0f; };

	gamerules["get_team_respawn_wave_time"] = [](int team_index) -> float
	{
		if (!TFGameRules() || team_index < 0 || team_index >= 32) return 0.0f;
		return TFGameRules()->m_TeamRespawnWaveTimes()[team_index];
	};

	// CTFGameRules
	gamerules["get_game_type"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nGameType() : 0; };
	gamerules["get_stopwatch_state"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nStopWatchState() : 0; };
	gamerules["get_team_goal_string_red"] = []() -> const char* { return TFGameRules() ? TFGameRules()->m_pszTeamGoalStringRed() : ""; };
	gamerules["get_team_goal_string_blue"] = []() -> const char* { return TFGameRules() ? TFGameRules()->m_pszTeamGoalStringBlue() : ""; };
	gamerules["get_capture_point_enable_time"] = []() -> float { return TFGameRules() ? TFGameRules()->m_flCapturePointEnableTime() : 0.0f; };
	gamerules["get_global_attribute_cache_version"] = []() -> int { return TFGameRules() ? TFGameRules()->m_iGlobalAttributeCacheVersion() : 0; };
	gamerules["get_hud_type"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nHudType() : 0; };
	gamerules["is_in_training"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bIsInTraining() : false; };
	gamerules["allow_training_achievements"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bAllowTrainingAchievements() : false; };
	gamerules["is_waiting_for_training_continue"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bIsWaitingForTrainingContinue() : false; };
	gamerules["is_training_hud_visible"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bIsTrainingHUDVisible() : false; };
	gamerules["is_in_item_testing_mode"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bIsInItemTestingMode() : false; };
	gamerules["get_bonus_logic_handle"] = []() -> CBaseEntity* { return TFGameRules() ? TFGameRules()->m_hBonusLogic().Get() : nullptr; };
	gamerules["is_playing_koth"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayingKoth() : false; };
	gamerules["is_powerup_mode"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPowerupMode() : false; };
	gamerules["is_playing_robot_destruction"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayingRobotDestructionMode() : false; };
	gamerules["is_playing_medieval"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayingMedieval() : false; };
	gamerules["is_playing_hybrid_ctf_cp"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayingHybrid_CTF_CP() : false; };
	gamerules["is_playing_special_delivery"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayingSpecialDeliveryMode() : false; };
	gamerules["is_playing_mann_vs_machine"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayingMannVsMachine() : false; };
	gamerules["get_mvm_alarm_status"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bMannVsMachineAlarmStatus() : false; };
	gamerules["have_min_players_to_enable_ready"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bHaveMinPlayersToEnableReady() : false; };
	gamerules["is_bounty_mode_enabled"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bBountyModeEnabled() : false; };
	gamerules["is_competitive_mode"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bCompetitiveMode() : false; };
	gamerules["get_match_group_type"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nMatchGroupType() : 0; };
	gamerules["has_match_ended"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bMatchEnded() : false; };
	gamerules["are_helltower_players_in_hell"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bHelltowerPlayersInHell() : false; };
	gamerules["is_using_spells"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bIsUsingSpells() : false; };
	gamerules["is_truce_active"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bTruceActive() : false; };
	gamerules["have_teams_switched"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bTeamsSwitched() : false; };
	gamerules["get_red_koth_timer_handle"] = []() -> CBaseEntity* { return TFGameRules() ? TFGameRules()->m_hRedKothTimer().Get() : nullptr; };
	gamerules["get_blue_koth_timer_handle"] = []() -> CBaseEntity* { return TFGameRules() ? TFGameRules()->m_hBlueKothTimer().Get() : nullptr; };
	gamerules["get_map_holiday_type"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nMapHolidayType() : 0; };
	gamerules["get_custom_upgrades_file"] = []() -> const char* { return TFGameRules() ? TFGameRules()->m_pszCustomUpgradesFile() : ""; };
	gamerules["show_match_summary"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bShowMatchSummary() : false; };
	gamerules["map_has_match_summary_stage"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bMapHasMatchSummaryStage() : false; };
	gamerules["players_are_on_match_summary_stage"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bPlayersAreOnMatchSummaryStage() : false; };
	gamerules["is_stopwatch_winner"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bStopWatchWinner() : false; };
	gamerules["player_wants_rematch"] = []() -> int { return TFGameRules() ? TFGameRules()->m_ePlayerWantsRematch() : 0; };
	gamerules["get_rematch_state"] = []() -> int { return TFGameRules() ? TFGameRules()->m_eRematchState() : 0; };
	gamerules["get_next_map_vote_options"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nNextMapVoteOptions() : 0; };
	gamerules["get_boss_health"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nBossHealth() : 0; };
	gamerules["get_max_boss_health"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nMaxBossHealth() : 0; };
	gamerules["get_boss_normalized_travel_distance"] = []() -> int { return TFGameRules() ? TFGameRules()->m_fBossNormalizedTravelDistance() : 0; };
	gamerules["get_it_handle"] = []() -> CBaseEntity* { return TFGameRules() ? TFGameRules()->m_itHandle().Get() : nullptr; };
	gamerules["get_birthday_player"] = []() -> CTFPlayer* { return TFGameRules() ? TFGameRules()->m_hBirthdayPlayer().Get() : nullptr; };
	gamerules["get_halloween_effect"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nHalloweenEffect() : 0; };
	gamerules["get_halloween_effect_start_time"] = []() -> float { return TFGameRules() ? TFGameRules()->m_fHalloweenEffectStartTime() : 0.0f; };
	gamerules["get_halloween_effect_duration"] = []() -> float { return TFGameRules() ? TFGameRules()->m_fHalloweenEffectDuration() : 0.0f; };
	gamerules["get_halloween_scenario"] = []() -> int { return TFGameRules() ? TFGameRules()->m_halloweenScenario() : 0; };
	gamerules["get_force_upgrades"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nForceUpgrades() : 0; };
	gamerules["get_force_escort_push_logic"] = []() -> int { return TFGameRules() ? TFGameRules()->m_nForceEscortPushLogic() : 0; };
	gamerules["are_ropes_holiday_lights_allowed"] = []() -> bool { return TFGameRules() ? TFGameRules()->m_bRopesHolidayLightsAllowed() : false; };
}