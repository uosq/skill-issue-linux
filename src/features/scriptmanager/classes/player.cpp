#include <vector>

#include "../../../thirdparty/sol3/sol.hpp"
#include "../../../sdk/classes/player.h"

#include "../../prediction/prediction.h"

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
		"GetSteamID3", &CTFPlayer::GetSteamID3,

		"PredictMovement", [](CTFPlayer* self, float seconds) -> std::optional<std::vector<Vec3>>
		{
			std::vector<Vec3> path;
			path.reserve(TIME_TO_TICKS(seconds));

			features::prediction.BeginPrediction(self, seconds);

			if (!features::prediction.Simulate(path))
			{
				features::prediction.EndPrediction();
				return std::nullopt;
			}

			features::prediction.EndPrediction();

			return path;
		}
	);
}