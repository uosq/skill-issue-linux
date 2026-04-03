#include "entitylist.h"

#include "../../../../../sdk/classes/entity.h"
#include "../../../../entitylist/entitylist.h"
#include "../../../add_on/scriptarray/scriptarray.h"
#include "../../globals.h"

CBaseEntity *EL_GetClientEntity(int index)
{
	return static_cast<CBaseEntity *>(interfaces::EntityList->GetClientEntity(index));
}

int EL_GetMaxEntities()
{
	return interfaces::EntityList->GetMaxEntities();
}

CBaseEntity *EL_GetLocalPlayer()
{
	return static_cast<CBaseEntity *>(EntityList::GetLocal());
}

bool EL_IsValid(int index)
{
	CBaseEntity *pEntity = static_cast<CBaseEntity *>(interfaces::EntityList->GetClientEntity(index));
	return pEntity != nullptr;
}

CScriptArray *EL_GetEntities()
{
	auto engine	  = GetScriptEngine();
	asITypeInfo *type = engine->GetTypeInfoByDecl("array<Entity@>");
	CScriptArray *arr = CScriptArray::Create(type);

	for (int i = 0; i < interfaces::EntityList->GetMaxEntities(); i++)
	{
		CBaseEntity *ent = static_cast<CBaseEntity *>(interfaces::EntityList->GetClientEntity(i));

		if (ent == nullptr)
			continue;

		arr->InsertLast(&ent);
	}

	return arr;
}

void Interface_EntityList_Register(asIScriptEngine *engine)
{
	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("EntityList");
	engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_ENTITYLIST);

	engine->RegisterGlobalFunction("Entity@ GetClientEntity(int entindex)", asFUNCTION(EL_GetClientEntity),
				       asCALL_CDECL);

	engine->RegisterGlobalFunction("int GetMaxEntities()", asFUNCTION(EL_GetMaxEntities), asCALL_CDECL);

	engine->RegisterGlobalFunction("Entity@ GetLocalPlayer()", asFUNCTION(EL_GetLocalPlayer), asCALL_CDECL);

	engine->RegisterGlobalFunction("array<Entity@>@ GetEntities()", asFUNCTION(EL_GetEntities),
				       asCALL_CDECL); // this one

	engine->SetDefaultNamespace(strDefaultNamespace);
}