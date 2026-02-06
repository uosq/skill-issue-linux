#include "../classes.h"
#include "../libraries.h"
#include "../../../sdk/definitions/imaterial.h"
#include "../../../sdk/definitions/itexture.h"

namespace LuaFuncs
{
	namespace render
	{
		const luaL_Reg renderlib[]
		{
			{"GetColorModulation", GetColorModulation},
			{"SetColorModulation", SetColorModulation},
			{"GetBlend", GetBlend},
			{"SetBlend", SetBlend},
			{"ForcedMaterialOverride", ForcedMaterialOverride},
			{"GetMaterialOverride", GetMaterialOverride},
			{"DrawScreenSpaceRectangle", DrawScreenSpaceRectangle},
			{"DrawScreenSpaceQuad", DrawScreenSpaceQuad},
			{"GetViewport", GetViewport},
			{"DepthRange", DepthRange},
			{"GetDepthRange", GetDepthRange},
			{"SetRenderTarget", SetRenderTarget},
			{"GetRenderTarget", GetRenderTarget},
			{"ClearBuffers", ClearBuffers},
			{"ClearColor3ub", ClearColor3ub},
			{"ClearColor4ub", ClearColor4ub},
			{"OverrideDepthEnable", OverrideDepthEnable},
			{"OverrideAlphaEnable", OverrideAlphaEnable},
			{"OverrideColorEnable", OverrideColorEnable},
			{"PushRenderTargetAndViewport", PushRenderTargetAndViewport},
			{"PopRenderTargetAndViewport", PopRenderTargetAndViewport},
			{"SetStencilEnable", SetStencilEnable},
			{"SetStencilFailOperation", SetStencilFailOperation},
			{"SetStencilZFailOperation", SetStencilZFailOperation},
			{"SetStencilPassOperation", SetStencilPassOperation},
			{"SetStencilCompareFunction", SetStencilCompareFunction},
			{"SetStencilReferenceValue", SetStencilReferenceValue},
			{"SetStencilTestMask", SetStencilTestMask},
			{"SetStencilWriteMask", SetStencilWriteMask},
			{"ClearStencilBufferRectangle", ClearStencilBufferRectangle},
			{"Viewport", Viewport},
			{nullptr, nullptr}
		};

		void luaopen_render(lua_State *L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, renderlib, 0);
			lua_setglobal(L, "render");
		}

		int GetColorModulation(lua_State* L)
		{
			float color[3] = {0, 0, 0};
			interfaces::RenderView->GetColorModulation(color);

			lua_pushnumber(L, color[0]);
			lua_pushnumber(L, color[1]);
			lua_pushnumber(L, color[2]);
			return 3;
		}

		int SetColorModulation(lua_State* L)
		{
			float r = luaL_optnumber(L, 1, 0.0f);
			float g = luaL_optnumber(L, 2, 0.0f);
			float b = luaL_optnumber(L, 3, 0.0f);

			r = std::clamp(r, 0.0f, 1.0f);
			g = std::clamp(g, 0.0f, 1.0f);
			b = std::clamp(b, 0.0f, 1.0f);

			float color[3] = {r, g, b};

			interfaces::RenderView->SetColorModulation(color);
			return 0;
		}

		int GetBlend(lua_State* L)
		{
			lua_pushnumber(L, interfaces::RenderView->GetBlend());
			return 1;
		}

		int SetBlend(lua_State* L)
		{
			float blend = luaL_optnumber(L, 1, 0.0f);
			interfaces::RenderView->SetBlend(blend);
			return 0;
		}

		int ForcedMaterialOverride(lua_State *L)
		{
			if (lua_isnil(L, 1))
			{
				interfaces::ModelRender->ForcedMaterialOverride(nullptr);
				return 0;
			}

			LuaMaterial* lmat = static_cast<LuaMaterial*>(luaL_checkudata(L, 1, "Material"));
			interfaces::ModelRender->ForcedMaterialOverride(lmat->mat);
			return 0;
		}

		int GetMaterialOverride(lua_State *L)
		{
			IMaterial* mat;
			OverrideType_t override;
			interfaces::ModelRender->GetMaterialOverride(&mat, &override);
			LuaClasses::MaterialLua::push_material(L, mat, mat->GetName());
			return 1;
		}

		int DrawScreenSpaceRectangle(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			LuaMaterial* lmat = static_cast<LuaMaterial*>(luaL_checkudata(L, 1, "Material"));
			if (lmat->mat == nullptr)
			{
				luaL_error(L, "Expected material, got nil!");
				return 0;
			}

			int destx = luaL_checkinteger(L, 2);
			int desty = luaL_checkinteger(L, 3);
			int width = luaL_checkinteger(L, 4);
			int height = luaL_checkinteger(L, 5);
			float src_texture_x0 = luaL_checknumber(L, 6);
			float src_texture_y0 = luaL_checknumber(L, 7);
			float src_texture_x1 = luaL_checknumber(L, 8);
			float src_texture_y1 = luaL_checknumber(L, 9);
			int src_texture_width = luaL_checkinteger(L, 10);
			int src_texture_height = luaL_checkinteger(L, 11);

			pRenderContext->DrawScreenSpaceRectangle(lmat->mat, destx, desty, width, height, src_texture_x0, src_texture_y0, src_texture_x1, src_texture_y1, src_texture_width, src_texture_height);
			return 0;
		}

		int DrawScreenSpaceQuad(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			LuaMaterial* lmat = static_cast<LuaMaterial*>(luaL_checkudata(L, 1, "Material"));
			if (lmat->mat == nullptr)
			{
				luaL_error(L, "Expected material, got nil!");
				return 0;
			}

			pRenderContext->DrawScreenSpaceQuad(lmat->mat);
			return 0;
		}

		int GetViewport(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int x, y, width, height;
			pRenderContext->GetViewport(x, y, width, height);

			lua_pushinteger(L, x);
			lua_pushinteger(L, y);
			lua_pushinteger(L, width);
			lua_pushinteger(L, height);
			return 4;
		}

		int DepthRange(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			float zNear = 0.0f, zFar = 1.0f;
			zNear = luaL_checknumber(L, 1);
			zFar = luaL_checknumber(L, 2);

			pRenderContext->DepthRange(zNear, zFar);
			return 0;
		}

		int GetDepthRange(lua_State* L)
		{
			CViewSetup view;
			if (!interfaces::ClientDLL->GetPlayerView(view))
			{
				lua_pushnil(L);
				lua_pushnil(L);
				return 2;
			}

			lua_pushinteger(L, view.zNear);
			lua_pushinteger(L, view.zFar);
			return 2;
		}

		int SetRenderTarget(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			LuaTexture* ltex = static_cast<LuaTexture*>(luaL_checkudata(L, 1, "Texture"));
			if (ltex->tex == nullptr)
			{
				luaL_error(L, "Expected Texture, received nil");
				return 0;
			}

			if (!ltex->tex->IsRenderTarget())
			{
				luaL_error(L, "Expected a Render Target Texture, received normal Texture (Did you use the wrong one?)");
				return 0;
			}

			pRenderContext->SetRenderTarget(ltex->tex);
			return 0;
		}

		int GetRenderTarget(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			ITexture* rendertarget = pRenderContext->GetRenderTarget();
			if (rendertarget == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::TextureLua::push_texture(L, rendertarget, rendertarget->GetName());
			return 1;
		}

		int ClearBuffers(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			bool bClearColor = luaL_checkinteger(L, 1);
			bool bClearDepth = luaL_checkinteger(L, 2);
			bool bClearStencil = luaL_optinteger(L, 3, 0);

			pRenderContext->ClearBuffers(bClearColor, bClearDepth, bClearStencil);

			return 0;
		}

		int ClearColor3ub(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			unsigned char r = static_cast<unsigned char>(luaL_checkinteger(L, 1));
			unsigned char g = static_cast<unsigned char>(luaL_checkinteger(L, 2));
			unsigned char b = static_cast<unsigned char>(luaL_checkinteger(L, 3));

			pRenderContext->ClearColor3ub(r, g, b);
			return 0;
		}

		int ClearColor4ub(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			unsigned char r = static_cast<unsigned char>(luaL_checkinteger(L, 1));
			unsigned char g = static_cast<unsigned char>(luaL_checkinteger(L, 2));
			unsigned char b = static_cast<unsigned char>(luaL_checkinteger(L, 3));
			unsigned char a = static_cast<unsigned char>(luaL_checkinteger(L, 4));

			pRenderContext->ClearColor4ub(r, g, b, a);
			return 0;
		}

		int OverrideDepthEnable(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			bool bEnable = luaL_checkinteger(L, 1);
			bool bDepthEnable = luaL_checkinteger(L, 2);
			pRenderContext->OverrideDepthEnable(bEnable, bDepthEnable);

			return 0;
		}

		int OverrideAlphaEnable(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			bool bEnable = luaL_checkinteger(L, 1);
			bool bAlphaWriteEnable = luaL_checkinteger(L, 2);

			pRenderContext->OverrideAlphaWriteEnable(bEnable, bAlphaWriteEnable);
			return 0;
		}

		int OverrideColorEnable(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			bool bEnable = luaL_checkinteger(L, 1);
			bool bColorWriteEnable = luaL_checkinteger(L, 2);

			pRenderContext->OverrideColorWriteEnable(bEnable, bColorWriteEnable);
			return 0;
		}

		int PushRenderTargetAndViewport(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			pRenderContext->PushRenderTargetAndViewport();
			return 0;
		}

		int PopRenderTargetAndViewport(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			pRenderContext->PopRenderTargetAndViewport();
			return 0;
		}

		int SetStencilEnable(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			bool state = luaL_checkinteger(L, 1);
			pRenderContext->SetStencilEnable(state);
			return 0;
		}

		int SetStencilFailOperation(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int op = luaL_checkinteger(L, 1);
			pRenderContext->SetStencilFailOperation(static_cast<StencilOperation_t>(op));
			return 0;
		}

		int SetStencilZFailOperation(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int op = luaL_checkinteger(L, 1);
			pRenderContext->SetStencilZFailOperation(static_cast<StencilOperation_t>(op));
			return 0;
		}

		int SetStencilPassOperation(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int op = luaL_checkinteger(L, 1);
			pRenderContext->SetStencilPassOperation(static_cast<StencilOperation_t>(op));
			return 0;
		}

		int SetStencilCompareFunction(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int op = luaL_checkinteger(L, 1);
			pRenderContext->SetStencilCompareFunction(static_cast<StencilComparisonFunction_t>(op));
			return 0;
		}

		int SetStencilReferenceValue(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int ref = luaL_checkinteger(L, 1);
			pRenderContext->SetStencilReferenceValue(ref);
			return 0;
		}

		int SetStencilTestMask(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			uint32_t mask = static_cast<uint32_t>(luaL_checkinteger(L, 1));
			pRenderContext->SetStencilTestMask(mask);
			return 0;
		}

		int SetStencilWriteMask(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			uint32_t mask = static_cast<uint32_t>(luaL_checkinteger(L, 1));
			pRenderContext->SetStencilWriteMask(mask);
			return 0;
		}

		int ClearStencilBufferRectangle(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int xmin = luaL_checkinteger(L, 1);
			int ymin = luaL_checkinteger(L, 2);
			int xmax = luaL_checkinteger(L, 3);
			int ymax = luaL_checkinteger(L, 4);
			int value = luaL_checkinteger(L, 5);

			pRenderContext->ClearStencilBufferRectangle(xmin, ymin, xmax, ymax, value);
			return 0;
		}

		int Viewport(lua_State* L)
		{
			auto pRenderContext = interfaces::MaterialSystem->GetRenderContext();
			if (pRenderContext == nullptr)
			{
				luaL_error(L, "Render context is nil!");
				return 0;
			}

			int x = luaL_checkinteger(L, 1);
			int y = luaL_checkinteger(L, 2);
			int width = luaL_checkinteger(L, 3);
			int height = luaL_checkinteger(L, 4);
			pRenderContext->Viewport(x, y, width, height);
			return 0;
		}
	}
}