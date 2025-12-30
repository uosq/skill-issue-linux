#pragma once

#include "../sdk/definitions/ipanel.h"
#include <cwchar>
#include <string>
#include "../sdk/classes/entity.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"
#include "../sdk/definitions/eteam.h"
#include "../sdk/definitions/color.h"

//using PaintTraverseFn = void (*)(IPanel* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce);
using PaintTraverseFn = void (*)(IEngineVGui* thisptr, VGuiPanel_t type);
inline PaintTraverseFn originalPaintTraverse = nullptr;

inline Color GetColor(Player* player)
{
	int team = player->m_iTeamNum();

	if (player->GetClassID() == ETFClassID::CTFPlayer)
	{
		switch (team)
		{
			case ETeam::TEAM_RED:
				return (Color){255, 0, 0, 255};
			case ETeam::TEAM_BLU:
				return (Color){0, 255, 255, 255};
			default:
				return (Color){255, 255, 255, 255};
		}
	}

	return (Color){255, 255, 255, 255};
}

inline void HookedEngineVGuiPaint(IEngineVGui* thisptr, VGuiPanel_t type)
{
	originalPaintTraverse(thisptr, type);

	//interfaces::surface->DrawSetColor(40, 40, 40, 255);
	//helper::draw::FilledRect(100, 100, 800, 800);

	if (!settings.enabled)
		return;

	HFont font = helper::draw::GetCurrentFont();
	helper::draw::SetFont(font);
	//helper::draw::SetColor(255, 255, 255, 255);
	//helper::draw::FilledRect(0, 0, 100, 100);

	Player* pLocal = helper::engine::GetLocalPlayer();
	if (!pLocal)
		return;

	Vector vecLocalOrigin = pLocal->m_vecOrigin();
	
	helper::draw::SetColor(255, 255, 255, 255);

	// start at 1 because 0 is the world
	for (int i = 1; i < helper::engine::GetMaxClients(); i++)
	{
		auto entity = (Player*)interfaces::entitylist->GetClientEntity(i);
		if (!entity)
			continue;

		if (entity == pLocal)
			continue;

		if (entity->IsDormant() || !entity->IsAlive())
			continue;

		//interfaces::vstdlib->ConsolePrintf("Health: %d\n", entity->GetHealth());

		Vector origin = entity->GetAbsOrigin();
		Vector feet; // fuck i need to sleep
		if (!helper::engine::WorldToScreen(origin, feet))
			continue;

		Vector head;
		if (!helper::engine::WorldToScreen(origin + Vector{0, 0, entity->m_vecMaxs().z}, head))
			continue;

		int h = (feet - head).Length();
		int w = h * 0.3;

		helper::draw::SetColor(GetColor(entity));
		helper::draw::OutlinedRect(head.x - w, head.y, feet.x + w, feet.y);
	}

	/*int width, height;
	std::wstring text = helper::draw::ConvertStringToWChar("Hello, world!");
	interfaces::surface->GetTextSize(font, text.c_str(), width, height);

	helper::draw::Text(50, 50, (Color){255, 255, 255, 255}, "Hello, world!\n");*/
}

inline void HookEngineVGuiPaint()
{
	auto vt = vtable::get(interfaces::enginevgui);
	originalPaintTraverse = vtable::hook(vt, 15, HookedEngineVGuiPaint);
	
	/*
		reminder to me:
		if using windows hooks, always add a +1, as (i think)
		gcc/clang puts the constructor&destructor at the start of the class
	*/

	Color_t color;
	color.SetRGB(100, 255, 100, 255);
	helper::console::ColoredPrint("EngineVGui::Paint hooked\n", color);
}