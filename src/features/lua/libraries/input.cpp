#include "../libraries.h"

namespace LuaFuncs
{
	namespace input
	{
		const luaL_Reg inputlib[]
		{
			{"IsButtonDown", IsButtonDown},
			{"IsButtonPressed", IsButtonPressed},
			{"IsButtonReleased", IsButtonReleased},
			{"SetCursorAlwaysVisible", SetCursorAlwaysVisible},
			{"IsCursorVisible", IsCursorVisible},
			{"GetPollTick", GetPollTick},
			{nullptr, nullptr}
		};

		void luaopen_input(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, inputlib, 0);
			lua_setglobal(L, "input");
		}

		int IsButtonDown(lua_State* L)
		{
			int btn = luaL_checkinteger(L, 1);
			ButtonCode_t button = static_cast<ButtonCode_t>(btn);
			if (button == BUTTON_CODE_INVALID)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			lua_pushboolean(L, interfaces::InputSystem->IsButtonDown(button));
			return 1;
		}

		int IsButtonPressed(lua_State* L)
		{
			int btn = luaL_checkinteger(L, 1);
			ButtonCode_t button = static_cast<ButtonCode_t>(btn);
			if (button == BUTTON_CODE_INVALID)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			bool down = interfaces::InputSystem->IsButtonDown(button);
			int tick = interfaces::InputSystem->GetButtonPressedTick(button);
			lua_pushboolean(L, down);
			lua_pushinteger(L, tick);
			return 2;
		}

		int IsButtonReleased(lua_State* L)
		{
			int btn = luaL_checkinteger(L, 1);
			ButtonCode_t button = static_cast<ButtonCode_t>(btn);
			if (button == BUTTON_CODE_INVALID)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			bool down = interfaces::InputSystem->IsButtonDown(button);
			int tick = interfaces::InputSystem->GetButtonReleasedTick(button);
			lua_pushboolean(L, down);
			lua_pushinteger(L, tick);
			return 2;
		}

		int SetCursorAlwaysVisible(lua_State* L)
		{
			bool state = luaL_checkinteger(L, 1);
			interfaces::Surface->SetCursorAlwaysVisible(state);
			return 0;
		}

		int IsCursorVisible(lua_State* L)
		{
			lua_pushboolean(L, interfaces::Surface->IsCursorVisible());
			return 1;
		}

		int GetCursorPos(lua_State* L)
		{
			int x, y;
			interfaces::Surface->SurfaceGetCursorPos(x, y);
			lua_pushinteger(L, x);
			lua_pushinteger(L, y);
			return 2;
		}

		int GetPollTick(lua_State* L)
		{
			lua_pushinteger(L, interfaces::InputSystem->GetPollTick());
			return 1;
		}
	}
}