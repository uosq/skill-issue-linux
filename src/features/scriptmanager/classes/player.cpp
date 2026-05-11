#include "../../../sol3/sol.hpp"

#include "../../../sdk/classes/player.h"

void BindPlayer(sol::state& lua)
{
	lua.new_usertype<CTFPlayer>
	(
		"Player", sol::no_constructor,

		sol::base_classes, sol::bases<CBaseEntity>(),

		"IsAlive", &CTFPlayer::IsAlive,
		"InCond", &CTFPlayer::InCond,
		"GetEyePos", &CTFPlayer::GetEyePos,
		"IsGhost", &CTFPlayer::IsGhost,
		"IsTaunting", &CTFPlayer::IsTaunting,
		"IsUbercharged", &CTFPlayer::IsUbercharged,
		"GetName", &CTFPlayer::GetName,
		"UpdateClientSideAnimation", &CTFPlayer::UpdateClientSideAnimation,
		"GetWaterLevel", &CTFPlayer::GetWaterLevel,
		"GetInvisibilityLevel", &CTFPlayer::GetEffectiveInvisibilityLevel,
		"GetMoveType", &CTFPlayer::GetMoveType,
		"GetEntityFromLoadoutSlot", &CTFPlayer::GetEntityFromLoadoutSlot,
		"GetUserID", &CTFPlayer::GetUserID,
		"GetSteamID", &CTFPlayer::GetSteamID,
		"GetSteamID3", &CTFPlayer::GetSteamID3
	);
}