#include "../libraries.h"
#include "../../../gui/console.h"

namespace LuaFuncs
{
	namespace common
	{
		void luaopen_commonfunctions(lua_State* L)
		{
			lua_register(L, "print", Print);
			lua_register(L, "typeof", Typeof);
		}

		int Print(lua_State* L)
		{
			int nargs = lua_gettop(L);
			if (nargs == 0)
				return 0;

			for (int i = 1; i <= nargs; i++)
			{
				const char* str = luaL_tolstring(L, i, nullptr);
				if (str)
				{
					consoleText += std::string(str) + "\n";
				}
			}

			return 0;
		}

		const char* GetMetaName(lua_State* L, int idx)
		{
			if (!lua_getmetatable(L, idx))
				return nullptr; // no metatable

			lua_getfield(L, -1, "__name");

			const char* name = nullptr;
			if (lua_isstring(L, -1))
				name = lua_tostring(L, -1);

			lua_pop(L, 2); // pop __name and metatable
			return name;
		}

		int Typeof(lua_State* L)
		{
			const char* name = GetMetaName(L, 1);
			if (name == nullptr)
			{
				lua_pushstring(L, lua_typename(L, lua_type(L, 1)));
				return 1;
			}

			lua_pushstring(L, name);
			return 1;
		}
	}
}