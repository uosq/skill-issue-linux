#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/ivengineclient.h"

void BindPlayerInfo(sol::state& lua)
{
	lua.new_usertype<player_info_t>
	(
		"PlayerInfo", sol::no_constructor,

		"is_htlv", &player_info_t::ishltv,
		"is_replay", &player_info_t::isreplay,
		"is_bot", &player_info_t::fakeplayer,
		"userid", &player_info_t::userID,
		"name", &player_info_t::name,
		"steamid", &player_info_t::guid
	);
}