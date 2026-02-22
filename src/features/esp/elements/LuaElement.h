#pragma once

#include <string>
#include "BaseElement.h"
#include "../../../settings/settings.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../gui/console.h"

#include "../../lua/api.h"
#include "../../lua/classes/esp_datalua.h"
#include "../../lua/classes/entitylua.h"

/*
esp.Register("hello world", function(entity, data)
	if entity is valid then
		return true
	end

	return false
end)
*/

class LuaElement : public IBaseElement
{
public:
	LuaElement()
	{
		m_text = "";
		m_id = "";
		m_alignment = ESP_ALIGNMENT::LEFT;
		m_shoulddraw_ref = LUA_NOREF;
	}

	LuaElement(const std::string& id, const std::string& text, int ref, ESP_ALIGNMENT alignment)
	{
		m_id = id;
		m_text = text;
		m_alignment = alignment;
		m_shoulddraw_ref = ref;
	}

	bool ShouldDraw(CBaseEntity* entity, const ESP_Data& data) const override
	{
		lua_State* L = Lua::m_luaState;

		if (!L || m_shoulddraw_ref == LUA_NOREF)
			return false;

		int top = lua_gettop(L);

		// push function
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_shoulddraw_ref);

		LuaClasses::EntityLua::push_entity(L, entity);

		ESP_Data copy = data;
		Lua_ESP_Data* ldata = LuaClasses::ESP_Data::push(L, copy);

		if (lua_pcall(L, 2, 1, 0) != LUA_OK)
		{
			const char* err = lua_tostring(L, -1);
			if (err)
				consoleText += std::string(err) + "\n";

			ldata->valid = false;
			lua_settop(L, top);
			return false;
		}

		ldata->valid = false;

		bool result = lua_toboolean(L, -1);

		// clean stack
		lua_settop(L, top);

		return result;
	}

	void Draw(Vec2& pos, CBaseEntity* ent, const ESP_Data& data, ESPContext& ctx) const override
	{
		helper::draw::TextShadow(pos.x, pos.y, Color(255, 255, 255, 255), m_text);
	}

	Vec2 GetSize(const ESP_Data& data) const override
	{
		int w, h;
		helper::draw::GetTextSize(m_text, w, h);
		return Vec2(w, h);
	}

	ESP_ALIGNMENT GetAlignment() const override
	{
		return m_alignment;
	}

	const std::string& GetText()
	{
		return m_text;
	}

	const std::string& GetID()
	{
		return m_id;
	}

	int GetRef()
	{
		return m_shoulddraw_ref;
	}

private:
	std::string m_text;
	int m_shoulddraw_ref;
	ESP_ALIGNMENT m_alignment;
	std::string m_id;
};