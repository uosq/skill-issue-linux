#include "entity.h"

#include "../../../../../sdk/classes/cbaseobject.h"
#include "../../../../../sdk/classes/ctfrobotdestruction_robot.h"
#include "../../../../../sdk/classes/player.h"
#include "../../../../entitylist/entitylist.h"

#include "../../globals.h"

#define ENTITY_CLASSNAME "Entity"

void Entity_AddRef(CBaseEntity *ent)
{
}
void Entity_Release(CBaseEntity *ent)
{
}

int GetHealth(CBaseEntity *ent)
{
	if (ent == nullptr)
		return 0;

	if (ent->IsPlayer())
	{
		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(ent);
		return pPlayer->GetHealth();
	}
	else if (ent->IsBuilding())
	{
		CBaseObject *pObj = static_cast<CBaseObject *>(ent);
		return pObj->m_iHealth();
	}
	else if (ent->IsRobot())
	{
		CTFRobotDestruction_Robot *pRobot = static_cast<CTFRobotDestruction_Robot *>(ent);
		return pRobot->m_iHealth();
	}

	return 0;
}

int GetMaxHealth(CBaseEntity *ent)
{
	if (ent == nullptr)
		return 0;

	if (ent->IsPlayer())
	{
		auto pResources = EntityList::GetPlayerResources();
		return pResources ? pResources->m_iMaxHealth(ent->GetIndex()) : 0;
	}
	else if (ent->IsBuilding())
	{
		CBaseObject *pObj = static_cast<CBaseObject *>(ent);
		return pObj->m_iMaxHealth();
	}
	else if (ent->IsRobot())
	{
		CTFRobotDestruction_Robot *pRobot = static_cast<CTFRobotDestruction_Robot *>(ent);
		return pRobot->m_iMaxHealth();
	}

	return 0;
}

bool Ent_IsPlayer(CBaseEntity *ent)
{
	return ent != nullptr ? ent->IsPlayer() : false;
}

template <typename T> T *GetProp(CBaseEntity *ent, const std::string &className, const std::string &prop)
{
	if (!ent)
		return nullptr;

	const auto &netvars = Netvars::m_netvarMap;

	auto hashed	    = fnv::Hash((className + "->" + prop).c_str());
	auto it		    = netvars.find(hashed);
	if (it == netvars.end())
		return nullptr;

	uintptr_t ptr = reinterpret_cast<uintptr_t>(ent);
	return reinterpret_cast<T *>(ptr + it->second);
}

template <typename T> void SetProp(CBaseEntity *ent, const std::string &className, const std::string &prop, T value)
{
	if (!ent)
		return;

	const auto &netvars = Netvars::m_netvarMap;

	auto hashed	    = fnv::Hash((className + "->" + prop).c_str());
	auto it		    = netvars.find(hashed);
	if (it == netvars.end())
	{
		auto engine = GetScriptEngine();
		engine->WriteMessage("Entity::SetProp", 0, 0, asMSGTYPE_ERROR, "Couldn't find netvar");
		return;
	}

	uintptr_t ptr				 = reinterpret_cast<uintptr_t>(ent);
	*reinterpret_cast<T *>(ptr + it->second) = value;
}

int GetPropInt(CBaseEntity *ent, const std::string &cls, const std::string &prop)
{
	if (ent == nullptr)
		return 0;
	return *GetProp<int>(ent, cls, prop);
}

float GetPropFloat(CBaseEntity *ent, const std::string &cls, const std::string &prop)
{
	if (ent == nullptr)
		return 0;
	return *GetProp<float>(ent, cls, prop);
}

bool GetPropBool(CBaseEntity *ent, const std::string &cls, const std::string &prop)
{
	if (ent == nullptr)
		return 0;
	return *GetProp<bool>(ent, cls, prop);
}

Vector GetPropVector(CBaseEntity *ent, const std::string &cls, const std::string &prop)
{
	if (ent == nullptr)
		return {};
	return *GetProp<Vector>(ent, cls, prop);
}

CBaseEntity *GetPropEntity(CBaseEntity *ent, const std::string &cls, const std::string &prop)
{
	if (ent == nullptr)
		return nullptr;
	EHANDLE handle = *GetProp<EHANDLE>(ent, cls, prop);
	auto realent   = HandleAs<CBaseEntity *>(handle);
	return realent;
}

void SetPropInt(CBaseEntity *ent, const std::string &cls, const std::string &prop, int value)
{
	if (ent == nullptr)
		return;
	SetProp(ent, cls, prop, value);
}

void SetPropFloat(CBaseEntity *ent, const std::string &cls, const std::string &prop, float value)
{
	if (ent == nullptr)
		return;
	SetProp(ent, cls, prop, value);
}

void SetPropVector(CBaseEntity *ent, const std::string &cls, const std::string &prop, Vector value)
{
	if (ent == nullptr)
		return;
	SetProp(ent, cls, prop, value);
}

void SetPropBool(CBaseEntity *ent, const std::string &cls, const std::string &prop, bool value)
{
	if (ent == nullptr)
		return;
	SetProp(ent, cls, prop, value);
}

void SetPropEntity(CBaseEntity *ent, const std::string &cls, const std::string &prop, CBaseEntity *entity)
{
	if (ent == nullptr)
		return;
	auto handle = entity->GetRefEHandle();
	SetProp(ent, cls, prop, handle);
}

Vector GetAbsOrigin(CBaseEntity *ent)
{
	if (ent == nullptr)
		return {};
	return ent->GetAbsOrigin();
}

Vector GetAbsAngles(CBaseEntity *ent)
{
	if (ent == nullptr)
		return {};
	return ent->GetAbsAngles();
}

bool ShouldDraw(CBaseEntity *ent)
{
	if (ent == nullptr)
		return false;
	return ent->ShouldDraw();
}

CBaseEntity *GetFirstMoveChild(CBaseEntity *ent)
{
	if (ent == nullptr)
		return nullptr;
	return static_cast<CBaseEntity *>(ent->FirstShadowChild());
}

CBaseEntity *GetNextMovePeer(CBaseEntity *ent)
{
	if (ent == nullptr)
		return nullptr;
	return static_cast<CBaseEntity *>(ent->NextShadowPeer());
}

int GetTeamNumber(CBaseEntity *ent)
{
	if (ent == nullptr)
		return 0;
	return ent->m_iTeamNum();
}

void DrawModel(CBaseEntity *ent, int flags)
{
	if (ent == nullptr)
		return;
	ent->DrawModel(flags);
}

bool Ent_IsAlive(CBaseEntity *ent)
{
	if (ent == nullptr)
		return false;

	if (ent->IsPlayer())
	{
		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(ent);
		return pPlayer->IsAlive();
	}

	if (ent->IsBuilding())
	{
		CBaseObject *pObj = static_cast<CBaseObject *>(ent);
		return pObj->m_iHealth() > 0;
	}

	if (ent->IsRobot())
	{
		CTFRobotDestruction_Robot *pRobot = static_cast<CTFRobotDestruction_Robot *>(ent);
		return pRobot->m_iHealth() > 0;
	}

	return false;
}

int GetMoveType(CBaseEntity *ent)
{
	if (ent == nullptr)
		return 0;

	if (ent->IsPlayer())
	{
		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(ent);
		return pPlayer->GetMoveType();
	}

	return 0;
}

bool InCond(CBaseEntity *ent, int cond)
{
	if (ent == nullptr)
		return false;

	if (ent->IsPlayer())
	{
		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(ent);
		return pPlayer->InCond(static_cast<ETFCond>(cond));
	}

	return false;
}

std::string GetClassName(CBaseEntity *ent)
{
	if (ent == nullptr)
		return "Invalid Entity";
	return ent->GetClientClass()->networkName;
}

std::string GetName(CBaseEntity *ent)
{
	if (ent == nullptr)
		return "";

	if (ent->IsPlayer())
	{
		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(ent);
		return pPlayer->GetName();
	}

	if (ent->IsBuilding())
	{
		CBaseObject *pObj = static_cast<CBaseObject *>(ent);
		return pObj->GetName();
	}

	return ent->GetClientClass()->networkName;
}

CBaseEntity *GetEntityFromLoadoutSlot(CBaseEntity *ent, int slot)
{
	if (ent == nullptr || !ent->IsPlayer())
		return nullptr;

	CTFPlayer *pPlayer = static_cast<CTFPlayer *>(ent);
	return reinterpret_cast<CTFPlayer *>(ent)->GetEntityFromLoadoutSlot(
	    slot /*g_SlotsToLoadoutSlotsPerClass[pPlayer->m_iClass()][slot]*/);
}

void Entity_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(ENTITY_CLASSNAME, 0, asOBJ_REF);

	engine->RegisterObjectBehaviour(ENTITY_CLASSNAME, asBEHAVE_ADDREF, "void f()", asFUNCTION(Entity_AddRef),
					asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour(ENTITY_CLASSNAME, asBEHAVE_RELEASE, "void f()", asFUNCTION(Entity_Release),
					asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "int GetHealth() const", asFUNCTION(GetHealth),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "int GetMaxHealth() const", asFUNCTION(GetMaxHealth),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool IsPlayer() const", asMETHOD(CBaseEntity, IsPlayer),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool IsWeapon() const", asMETHOD(CBaseEntity, IsWeapon),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "int GetIndex() const", asMETHOD(CBaseEntity, GetIndex),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool IsPlayer()", asMETHOD(CBaseEntity, IsPlayer),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool IsBuilding()", asMETHOD(CBaseEntity, IsBuilding),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "int GetClassID() const", asMETHOD(CBaseEntity, GetClassID),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool IsRobot()", asMETHOD(CBaseEntity, IsRobot),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "Vector3 EstimateAbsVelocity() const",
				     asMETHOD(CBaseEntity, EstimateAbsVelocity), asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool IsProjectile()", asMETHOD(CBaseEntity, IsProjectile),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "int GetPropInt(const string &in "
				     "className, const string &in propName)",
				     asFUNCTION(GetPropInt), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "float GetPropFloat(const string &in "
				     "className, const string &in propName)",
				     asFUNCTION(GetPropFloat), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "bool GetPropBool(const string &in "
				     "className, const string &in propName)",
				     asFUNCTION(GetPropBool), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "Vector3 GetPropVector(const string &in "
				     "className, const string &in propName)",
				     asFUNCTION(GetPropVector), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "Entity@ GetPropEntity(const string &in "
				     "className, const string &in propName)",
				     asFUNCTION(GetPropEntity), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "void SetPropInt(const string &in className, const string &in "
				     "propName, int value)",
				     asFUNCTION(SetPropInt), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "void SetPropFloat(const string &in className, const string &in "
				     "propName, float value)",
				     asFUNCTION(SetPropFloat), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "void SetPropBool(const string &in className, const string &in "
				     "propName, bool value)",
				     asFUNCTION(SetPropBool), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "void SetPropVector(const string &in className, const string &in "
				     "propName, Vector3 value)",
				     asFUNCTION(SetPropVector), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME,
				     "void SetPropEntity(const string &in className, const string &in "
				     "propName, Entity@ value)",
				     asFUNCTION(SetPropEntity), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "Vector3 GetAbsOrigin()", asFUNCTION(GetAbsOrigin),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "Vector3 GetAbsAngles()", asFUNCTION(GetAbsAngles),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool ShouldDraw()", asFUNCTION(ShouldDraw),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "Entity@ GetFirstMoveChild()", asFUNCTION(GetFirstMoveChild),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "Entity@ GetNextMovePeer()", asFUNCTION(GetNextMovePeer),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "void DrawModel(int flags)", asFUNCTION(DrawModel),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "int GetTeamNumber()", asFUNCTION(GetTeamNumber),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool IsAlive()", asFUNCTION(Ent_IsAlive),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "int GetMoveType()", asFUNCTION(GetMoveType),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "bool InCond(int cond)", asFUNCTION(InCond),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "string GetClassName()", asFUNCTION(GetClassName),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "string GetName()", asFUNCTION(GetName), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(ENTITY_CLASSNAME, "Entity@ GetEntityFromLoadoutSlot(int index)",
				     asFUNCTION(GetEntityFromLoadoutSlot), asCALL_CDECL_OBJFIRST);
}