#include "playerinfo.h"

#include "../../../../../sdk/definitions/ivengineclient.h"

void PlayerInfo_RegisterClass(asIScriptEngine* engine)
{
	constexpr const char* CLASSNAME = "PlayerInfo";

	engine->RegisterObjectType(CLASSNAME, sizeof(player_info_t), asOBJ_POD | asOBJ_VALUE);
	engine->RegisterObjectProperty(CLASSNAME, "int userID", asOFFSET(player_info_t, userID));
	engine->RegisterObjectProperty(CLASSNAME, "bool fakeplayer", asOFFSET(player_info_t, fakeplayer));
	engine->RegisterObjectProperty(CLASSNAME, "bool ishltv", asOFFSET(player_info_t, ishltv));
	engine->RegisterObjectProperty(CLASSNAME, "bool isreplay", asOFFSET(player_info_t, isreplay));
}