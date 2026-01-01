# pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"
#include "../../../sdk/handle_utils.h"
#include <vector>

struct SpectatorList{
	void Run(CTFPlayer* pLocal)
	{
		int ourIndex = pLocal->GetIndex();
		int ourTeam = pLocal->m_iTeamNum();
		int count = 0;

		Color normal{255, 255, 255, 255};
		Color firstperson{255, 100, 100, 255};

		for (int i = 1; i < helper::engine::GetMaxClients(); i++)
		{
			CTFPlayer* player = (CTFPlayer*)interfaces::entitylist->GetClientEntity(i);
			if (!player || !player->IsPlayer() || player->IsAlive())
				continue;

			if (player->m_iTeamNum() != ourTeam)
				continue;

			CTFPlayer* m_hObserverTarget = HandleAs<CTFPlayer>(player->m_hObserverTarget());
			if (m_hObserverTarget->GetIndex() != ourIndex)
				continue;

			player_info_t info;
			if (!interfaces::engine->GetPlayerInfo(i, &info))
				continue;

			int m_iObserverMode = player->m_iObserverMode();
			bool isfirstperson = m_iObserverMode == OBS_MODE_IN_EYE;
			helper::draw::TextShadow(10, 100 + (18 * i), isfirstperson ? firstperson : normal, info.name);
			count++;
		}
	}
};