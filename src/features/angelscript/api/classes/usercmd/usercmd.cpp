#include "usercmd.h"

#include "../../../../../sdk/definitions/cusercmd.h"
#include "../../../../ticks/ticks.h"

#define USERCMD_CLASSNAME "UserCmd"

bool UserCmd_GetSendPacket(CUserCmd *self)
{
	if (!self)
		return false;
	return TickManager::m_bSendPacket;
}

void UserCmd_SetSendPacket(CUserCmd *self, bool state)
{
	if (!self)
		return;
	TickManager::m_bSendPacket = state;
}

void UserCmd_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(USERCMD_CLASSNAME, 0, asOBJ_REF | asOBJ_NOCOUNT);

	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int command_number", asOFFSET(CUserCmd, command_number));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int tick_count", asOFFSET(CUserCmd, tick_count));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "Vector3 viewangles", asOFFSET(CUserCmd, viewangles));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "float forwardmove", asOFFSET(CUserCmd, forwardmove));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "float sidemove", asOFFSET(CUserCmd, sidemove));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "float upmove", asOFFSET(CUserCmd, upmove));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int buttons", asOFFSET(CUserCmd, buttons));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int impulse", asOFFSET(CUserCmd, impulse));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int weaponselect", asOFFSET(CUserCmd, weaponselect));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int weaponsubtype", asOFFSET(CUserCmd, weaponsubtype));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int random_seed", asOFFSET(CUserCmd, random_seed));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int mousedx", asOFFSET(CUserCmd, mousedx));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "int mousedy", asOFFSET(CUserCmd, mousedy));
	engine->RegisterObjectProperty(USERCMD_CLASSNAME, "bool hasbeenpredicted",
				       asOFFSET(CUserCmd, hasbeenpredicted));

	engine->RegisterObjectMethod(USERCMD_CLASSNAME, "bool GetSendPacket() const", asFUNCTION(UserCmd_GetSendPacket),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(USERCMD_CLASSNAME, "void SetSendPacket(bool state)",
				     asFUNCTION(UserCmd_SetSendPacket), asCALL_CDECL_OBJFIRST);
}