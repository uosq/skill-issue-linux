#include "materials.h"
#include <string>

#include "../../../../../sdk/MaterialManager/materialmanager.h"
#include "../../globals.h"

IMaterial *CreateMat(const std::string &name, const std::string &vmt)
{
	return MaterialManager::CreateMaterial(name, vmt);
}

/*bool FreeMat(const std::string &name)
{
	return MaterialManager::FreeMaterial(name);
}*/

IMaterial *GetMat(const std::string &name)
{
	return MaterialManager::GetMaterial(name);
}

ITexture *CreateTextureNamedRenderTarget(const std::string &name, int width, int height)
{
	return MaterialManager::CreateTextureNamedRenderTarget(name, width, height);
}

void Materials_RegisterLibrary(asIScriptEngine *engine)
{
	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Materials");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_MATERIALS);
		engine->RegisterGlobalFunction("Material@ CreateMaterial(const string &in name, const "
					       "string &in vmt)",
					       asFUNCTION(CreateMat), asCALL_CDECL);
		// engine->RegisterGlobalFunction("bool FreeMaterial(const
		// string &in name)", asFUNCTION(FreeMat), asCALL_CDECL);
		engine->RegisterGlobalFunction("Material@ GetMaterial(const string &in name)", asFUNCTION(GetMat),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("Texture@ CreateTextureNamedRenderTarget(const string &in "
					       "name, int width, int height)",
					       asFUNCTION(CreateTextureNamedRenderTarget), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(strDefaultNamespace);
}