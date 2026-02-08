#include "../libraries.h"
#include "../../../sdk/MaterialManager/materialmanager.h"
#include "../classes.h"

namespace LuaFuncs
{
	namespace materials
	{
		const luaL_Reg matslib[]
		{
			{"CreateMaterial", Create},
			{"GetMaterial", GetMaterial},
			{"GetTexture", GetTexture},
			{"FindMaterial", FindMaterial},
			{"FindTexture", FindTexture},
			{"CreateTextureRenderTarget", CreateTextureRenderTarget},
			{nullptr, nullptr}
		};

		void luaopen_materials(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, matslib, 0);
			lua_setglobal(L, "materials");
		}

		// materials.Create("name", "vmt")
		int Create(lua_State* L)
		{
			const char* name = luaL_checkstring(L, 1);
			if (name == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* vmt = luaL_checkstring(L, 2);
			if (vmt == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			IMaterial* mat = MaterialManager::CreateMaterial(name, vmt);
			if (mat == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}
			
			LuaClasses::MaterialLua::push_material(L, mat, name);
			return 1;
		}

		int FindMaterial(lua_State* L)
		{
			const char* name = luaL_checkstring(L, 1);
			const char* groupName = luaL_checkstring(L, 2);
			bool complain = luaL_optinteger(L, 3, 0);
			IMaterial* mat = interfaces::MaterialSystem->FindMaterial(name, groupName, complain);
			if (mat == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::MaterialLua::push_material(L, mat, name);
			return 1;
		}

		int FindTexture(lua_State* L)
		{
			const char* name = luaL_checkstring(L, 1);
			const char* groupName = luaL_checkstring(L, 2);
			bool complain = luaL_optinteger(L, 3, 1);

			ITexture* tex = interfaces::MaterialSystem->FindTexture(name, groupName, complain);
			if (tex == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::TextureLua::push_texture(L, tex, name);
			return 1;
		}

		int CreateTextureRenderTarget(lua_State* L)
		{
			const char* name = luaL_checkstring(L, 1);
			int w = luaL_checkinteger(L, 2);
			int h = luaL_checkinteger(L, 3);

			ITexture* tex = MaterialManager::CreateTextureNamedRenderTarget(name, w, h);
			if (tex != nullptr)
			{
				LuaClasses::TextureLua::push_texture(L, tex, name);
				return 1;
			}

			lua_pushnil(L);
			return 1;
		}

		int GetMaterial(lua_State *L)
		{
			const char* name = luaL_checkstring(L, 1);
			IMaterial* mat = MaterialManager::GetMaterial(name);
			if (mat == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::MaterialLua::push_material(L, mat, name);
			return 1;
		}

		int GetTexture(lua_State *L)
		{
			const char* name = luaL_checkstring(L, 1);
			ITexture* tex = MaterialManager::GetTexture(name);
			if (tex == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::TextureLua::push_texture(L, tex, name);
			return 1;
		}
	}
}