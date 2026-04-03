#include "render.h"

#include "../../../../../sdk/definitions/imaterial.h"
#include "../../../../../sdk/definitions/itexture.h"
#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../globals.h"

void GetColorMod(float &r, float &g, float &b)
{
	float color[3];

	interfaces::RenderView->GetColorModulation(color);

	r = color[0];
	g = color[1];
	b = color[2];
}

float GetBlend()
{
	return interfaces::RenderView->GetBlend();
}

void SetColorMod(float r, float g, float b)
{
	float color[]{r, g, b};
	interfaces::RenderView->SetColorModulation(color);
}

void SetBlend(float blend)
{
	interfaces::RenderView->SetBlend(blend);
}

void ForcedMaterialOverride(IMaterial *mat)
{
	if (!mat)
		return interfaces::StudioRender->ForcedMaterialOverride(nullptr);
	interfaces::StudioRender->ForcedMaterialOverride(mat);
}

IMaterial *GetMaterialOverride()
{
	IMaterial *mat = nullptr;
	interfaces::StudioRender->GetMaterialOverride(&mat, nullptr);
	return (mat && !mat->IsErrorMaterial()) ? mat : nullptr;
}

void DrawScreenSpaceRectangle(IMaterial *mat, int destx, int desty, int width, int height, float src_texture_x0,
			      float src_texture_y0, float src_texture_x1, float src_texture_y1, int src_texture_width,
			      int src_texture_height)
{
	if (!mat)
		return;

	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->DrawScreenSpaceRectangle(mat, destx, desty, width, height, src_texture_x0, src_texture_y0, src_texture_x1,
				      src_texture_y1, src_texture_width, src_texture_height);
}

void DrawScreenSpaceQuad(IMaterial *mat)
{
	if (!mat)
		return;

	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->DrawScreenSpaceQuad(mat);
}

void SetRenderTarget(ITexture *tex)
{
	if (!tex || !tex->IsRenderTarget())
		return;

	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetRenderTarget(tex);
}

ITexture *GetRenderTarget()
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return nullptr;

	return ctx->GetRenderTarget();
}

void ClearBuffers(bool clearColor, bool clearDepth, bool clearStencil = false)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->ClearBuffers(clearColor, clearDepth, clearStencil);
}

void ClearColor3ub(uint8_t r, uint8_t g, uint8_t b)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->ClearColor3ub(r, g, b);
}

void ClearColor4ub(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->ClearColor4ub(r, g, b, a);
}

void OverrideDepthEnable(bool enable, bool depthEnable)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->OverrideDepthEnable(enable, depthEnable);
}

void OverrideAlphaEnable(bool enable, bool alphaWrite)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->OverrideAlphaWriteEnable(enable, alphaWrite);
}

void OverrideColorEnable(bool enable, bool colorWrite)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->OverrideColorWriteEnable(enable, colorWrite);
}

void PushRenderTargetAndViewport()
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->PushRenderTargetAndViewport();
}

void PopRenderTargetAndViewport()
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->PopRenderTargetAndViewport();
}

void SetStencilEnable(bool state)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilEnable(state);
}

void SetStencilFailOperation(int op)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilFailOperation((StencilOperation_t)op);
}

void SetStencilZFailOperation(int op)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilZFailOperation((StencilOperation_t)op);
}

void SetStencilPassOperation(int op)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilPassOperation((StencilOperation_t)op);
}

void SetStencilCompareFunction(int func)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilCompareFunction((StencilComparisonFunction_t)func);
}

void SetStencilReferenceValue(int ref)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilReferenceValue(ref);
}

void SetStencilTestMask(uint32_t mask)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilTestMask(mask);
}

void SetStencilWriteMask(uint32_t mask)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->SetStencilWriteMask(mask);
}

void ClearStencilBufferRectangle(int xmin, int ymin, int xmax, int ymax, int value)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->ClearStencilBufferRectangle(xmin, ymin, xmax, ymax, value);
}

void Viewport(int x, int y, int width, int height)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->Viewport(x, y, width, height);
}

void GetViewport(int &x, int &y, int &width, int &height)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->GetViewport(x, y, width, height);
}

void DepthRange(float zNear, float zFar)
{
	auto ctx = interfaces::MaterialSystem->GetRenderContext();
	if (!ctx)
		return;

	ctx->DepthRange(zNear, zFar);
}

bool GetDepthRange(float &zNear, float &zFar)
{
	CViewSetup view;
	if (!interfaces::ClientDLL->GetPlayerView(view))
		return false;

	zNear = view.zNear;
	zFar  = view.zFar;
	return true;
}

void Render_RegisterLibrary(asIScriptEngine *engine)
{
	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Render");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_RENDER);
		engine->RegisterGlobalFunction("void GetColorModulation(float &out r, float &out g, "
					       "float &out b)",
					       asFUNCTION(GetColorMod), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetColorModulation(float r, float g, float b)",
					       asFUNCTION(SetColorMod), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetBlend()", asFUNCTION(GetBlend), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetBlend(float blend)", asFUNCTION(SetBlend), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ForcedMaterialOverride(Material@ mat)",
					       asFUNCTION(ForcedMaterialOverride), asCALL_CDECL);
		engine->RegisterGlobalFunction("Material@ GetMaterialOverride()", asFUNCTION(GetMaterialOverride),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void DrawScreenSpaceRectangle(Material@, int, int, int, "
					       "int, float, float, float, float, int, int)",
					       asFUNCTION(DrawScreenSpaceRectangle), asCALL_CDECL);
		engine->RegisterGlobalFunction("void DrawScreenSpaceQuad(Material@)", asFUNCTION(DrawScreenSpaceQuad),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void GetViewport(int &out, int &out, int &out, int &out)",
					       asFUNCTION(GetViewport), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Viewport(int, int, int, int)", asFUNCTION(Viewport), asCALL_CDECL);
		engine->RegisterGlobalFunction("void DepthRange(float, float)", asFUNCTION(DepthRange), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool GetDepthRange(float &out, float &out)", asFUNCTION(GetDepthRange),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetRenderTarget(Texture@)", asFUNCTION(SetRenderTarget),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("Texture@ GetRenderTarget()", asFUNCTION(GetRenderTarget), asCALL_CDECL);
		engine->RegisterGlobalFunction("void PushRenderTargetAndViewport()",
					       asFUNCTION(PushRenderTargetAndViewport), asCALL_CDECL);
		engine->RegisterGlobalFunction("void PopRenderTargetAndViewport()",
					       asFUNCTION(PopRenderTargetAndViewport), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ClearBuffers(bool, bool, bool = false)", asFUNCTION(ClearBuffers),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void ClearColor3ub(uint, uint, uint)", asFUNCTION(ClearColor3ub),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void ClearColor4ub(uint, uint, uint, uint)", asFUNCTION(ClearColor4ub),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void OverrideDepthEnable(bool, bool)", asFUNCTION(OverrideDepthEnable),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void OverrideAlphaEnable(bool, bool)", asFUNCTION(OverrideAlphaEnable),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void OverrideColorEnable(bool, bool)", asFUNCTION(OverrideColorEnable),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilEnable(bool)", asFUNCTION(SetStencilEnable),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilFailOperation(int)", asFUNCTION(SetStencilFailOperation),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilZFailOperation(int)",
					       asFUNCTION(SetStencilZFailOperation), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilPassOperation(int)", asFUNCTION(SetStencilPassOperation),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilCompareFunction(int)",
					       asFUNCTION(SetStencilCompareFunction), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilReferenceValue(int)",
					       asFUNCTION(SetStencilReferenceValue), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilTestMask(uint)", asFUNCTION(SetStencilTestMask),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStencilWriteMask(uint)", asFUNCTION(SetStencilWriteMask),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void ClearStencilBufferRectangle(int, int, int, int, "
					       "int)",
					       asFUNCTION(ClearStencilBufferRectangle), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(strDefaultNamespace);
}