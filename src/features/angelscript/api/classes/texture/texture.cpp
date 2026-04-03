#include "texture.h"

#include "../../../../../sdk/MaterialManager/materialmanager.h"
#include "../../../../../sdk/definitions/itexture.h"

#define TEXTURE_CLASSNAME "Texture"

bool Delete(ITexture *tex)
{
	if (!tex)
		return false;
	return MaterialManager::FreeTexture(tex->GetName());
}

std::string GetName(ITexture *tex)
{
	return tex ? tex->GetName() : "Invalid Texture";
}

unsigned int GetFlags(ITexture *tex)
{
	if (!tex)
		return 0;
	return tex->GetFlags();
}

int GetActualWidth(ITexture *tex)
{
	if (!tex)
		return 0;
	return tex->GetActualWidth();
}

int GetActualHeight(ITexture *tex)
{
	if (!tex)
		return 0;
	return tex->GetActualHeight();
}

bool IsTranslucent(ITexture *tex)
{
	if (!tex)
		return false;
	return tex->IsTranslucent();
}

bool IsRenderTarget(ITexture *tex)
{
	if (!tex)
		return false;
	return tex->IsRenderTarget();
}

void Texture_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(TEXTURE_CLASSNAME, 0, asOBJ_REF | asOBJ_NOCOUNT);

	engine->RegisterObjectMethod(TEXTURE_CLASSNAME, "bool Delete()", asFUNCTION(Delete), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(TEXTURE_CLASSNAME, "string GetName()", asFUNCTION(GetName), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(TEXTURE_CLASSNAME, "uint GetFlags()", asFUNCTION(GetFlags), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(TEXTURE_CLASSNAME, "int GetActualWidth()", asFUNCTION(GetActualWidth),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(TEXTURE_CLASSNAME, "int GetActualHeight()", asFUNCTION(GetActualHeight),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(TEXTURE_CLASSNAME, "bool IsTranslucent()", asFUNCTION(IsTranslucent),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(TEXTURE_CLASSNAME, "bool IsRenderTarget()", asFUNCTION(IsRenderTarget),
				     asCALL_CDECL_OBJFIRST);
}