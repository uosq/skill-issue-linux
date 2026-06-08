#include "nopush.h"

#include <cstring>

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/definitions/inetmessage.h"

#include "../../settings/settings.h"

void CNoPush::OnFrameStageNotify()
{
	static ConVar* tf_avoidteammates_pushaway = interfaces::Cvar->FindVar("tf_avoidteammates_pushaway");

	bool should_nopush = Config.misc.packed.nopush;
	bool can_be_pushed = tf_avoidteammates_pushaway->GetBool();
	
	if (should_nopush)
	{
		if (can_be_pushed)
			tf_avoidteammates_pushaway->SetValue(0);
	}
	else
	{
		if (!can_be_pushed)
			tf_avoidteammates_pushaway->SetValue(1);
	}
}

void CNoPush::OnSendNetMsg(INetMessage& msg)
{
	if (!Config.misc.packed.nopush)
		return;

	NET_SetConVar& cvars = reinterpret_cast<NET_SetConVar&>(msg);

	int count = cvars.m_ConVars.Count();
	if (count <= 0) return;
	
	for (int i = 0; i < count; i++)
	{
		NET_SetConVar::CVar_t& cvar = cvars.m_ConVars.Element(i);

		if (std::strcmp(cvar.Name, "tf_avoidteammates_pushaway") != 0)
			continue;

		if (std::strcmp(cvar.Value, "0") == 0)
			continue;

		std::strncpy(cvar.Value, "1", MAX_OSPATH);
	}
}