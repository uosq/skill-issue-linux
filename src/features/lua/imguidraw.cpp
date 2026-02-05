#include "classes.h"

/*namespace LuaClasses
{
	namespace ImGuiDraw
	{
		const luaL_Reg methods[]
		{
			{"FillRect", FillRect},
			{nullptr, nullptr}
		};

		void open(lua_State* L)
		{
			luaL_newmetatable(L, "ImGuiDrawList");
			luaL_setfuncs(L, methods, 0);

			lua_pushcfunction(L, Index);
			lua_setfield(L, -2, "__index");
		}

		LuaImGuiDraw* push_drawlist(lua_State* L, ImDrawList* draw);
		int Index(lua_State* L);
		
		// methods
		int FillRect(lua_State* L);
	}
}*/