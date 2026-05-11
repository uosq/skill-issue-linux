#include "../../../sol3/sol.hpp"

#include "../../../sdk/classes/entity.h"
#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"

void BindEntity(sol::state& lua)
{
	lua.new_usertype<CBaseEntity>
	(
		"Entity", sol::no_constructor,

		"GetAbsOrigin", &CBaseEntity::GetAbsOrigin,
		"SetAbsOrigin", &CBaseEntity::SetAbsOrigin,
		"SetAbsAngles", &CBaseEntity::SetAbsAngles,
		"GetAbsAngles", &CBaseEntity::GetAbsAngles,
		"IsPlayer", &CBaseEntity::IsPlayer,
		"IsWeapon", &CBaseEntity::IsWeapon,
		"GetIndex", &CBaseEntity::entindex,
		"GetCenter", &CBaseEntity::GetCenter,
		"EstimateAbsVelocity", &CBaseEntity::EstimateAbsVelocity,
		"IsProjectile", &CBaseEntity::IsProjectile,
		"SetupBones", &CBaseEntity::SetupBones,
		"DrawModel", &CBaseEntity::DrawModel,
		"ShouldDraw", &CBaseEntity::ShouldDraw,

		"GetClassID", [](CBaseEntity* self)
		{
			// gotta convert ts
			return static_cast<int>(self->GetClassID());
		},

		"GetTeamNumber", [](CBaseEntity* self)
		{
			return self->m_iTeamNum();
		},

		"GetClassName", [](CBaseEntity* self)
		{
			ClientClass* clientclass = self->GetClientClass();

			if (!clientclass)
				return "Unknown class";

			const char* name = clientclass->networkName;

			if (name == nullptr || name[0] == '\0')
				return "Unknown class";

			return name;
		},

		"ToPlayer", [](CBaseEntity* self) -> CTFPlayer*
		{
			if (!self->IsPlayer())
				return nullptr;

			return static_cast<CTFPlayer*>(self);
		},

		"ToEntity", [](CBaseEntity* self) -> CBaseEntity*
		{
			return static_cast<CBaseEntity*>(self);
		},

		"ToWeapon", [](CBaseEntity* self) -> CTFWeaponBase*
		{
			if (!self->IsWeapon())
				return nullptr;

			return static_cast<CTFWeaponBase*>(self);
		},

		"AttributeHookInt", [](CBaseEntity* self, const std::string& attrName,std::optional<int> defValue)
		{
			return static_cast<int>(AttributeHookValue(defValue.value_or(1.0f), attrName.c_str(), self, nullptr, false));
		},

		"AttributeHookFloat", [](CBaseEntity* self, const std::string& attrName,std::optional<int> defValue)
		{
			return AttributeHookValue(defValue.value_or(1.0f), attrName.c_str(), self, nullptr, false);
		},

		"IsValid", [](CBaseEntity* self)
		{
			return self != nullptr;
		},

		"IsClientEntity", [](CBaseEntity* self)
		{
			return self->GetIndex() < 0;
		},

		"GetNetvarInt", [](CBaseEntity* self, const std::string& className, const std::string& prop)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			return *reinterpret_cast<int *>(reinterpret_cast<uintptr_t>(self) + offset);
		},

		"GetNetvarFloat", [](CBaseEntity* self, const std::string& className, const std::string& prop)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			return *reinterpret_cast<float *>(reinterpret_cast<uintptr_t>(self) + offset);
		},

		"GetNetvarVector", [](CBaseEntity* self, const std::string& className, const std::string& prop)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			return *reinterpret_cast<Vector *>(reinterpret_cast<uintptr_t>(self) + offset);
		},

		"GetNetvarEntity", [](CBaseEntity* self, const std::string& className, const std::string& prop)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			EHANDLE handle = *reinterpret_cast<EHANDLE *>(reinterpret_cast<uintptr_t>(self) + offset);
			return HandleAs<CBaseEntity*>(handle);
		},

		"SetNetvarInt", [](CBaseEntity* self, const std::string& className, const std::string& prop, int value)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			int* val = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(self) + offset);
			*val = value;
		},

		"SetNetvarFloat", [](CBaseEntity* self, const std::string& className, const std::string& prop, float value)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			float* val = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(self) + offset);
			*val = value;
		},

		"SetNetvarVector", [](CBaseEntity* self, const std::string& className, const std::string& prop, const Vector& value)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			Vector* val = reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(self) + offset);
			val->Set(value.x, value.y, value.z);
		},

		"SetNetvarEntity", [](CBaseEntity* self, const std::string& className, const std::string& prop, CBaseEntity* value)
		{
			std::string full = className + "->" + prop;
			auto offset = Netvars ::m_netvarMap[fnv::Hash(full.c_str())];
			EHANDLE handle = value->GetRefEHandle();
			EHANDLE* val = reinterpret_cast<EHANDLE*>(reinterpret_cast<uintptr_t>(self) + offset);
			*val = handle;
		}
	);
}