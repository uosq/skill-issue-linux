#include "../../../sol3/sol.hpp"

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/classes/player.h"

void BindEntities(sol::state& lua)
{
	auto entities = lua.create_named_table("entities");

	entities["GetClientEntity"] = [](int entnum) -> CBaseEntity*
	{
		IClientEntity* clientEntity = interfaces::EntityList->GetClientEntity(entnum);

		if (clientEntity == nullptr)
			return nullptr;

		CBaseEntity* entity = clientEntity->GetBaseEntity();
		return entity;
	};

	entities["GetMaxEntities"] = []() -> int
	{
		return interfaces::EntityList->GetMaxEntities();
	};

	entities["GetLocalPlayer"] = []() -> CTFPlayer*
	{
		int index = interfaces::Engine->GetLocalPlayer();

		if (index <= 0)
			return nullptr;

		IClientEntity* clientEntity = interfaces::EntityList->GetClientEntity(index);

		if (clientEntity == nullptr)
			return nullptr;

		return static_cast<CTFPlayer*>(clientEntity->GetBaseEntity());
	};
}