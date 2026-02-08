#pragma once

#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/definitions/imaterial.h"
#include "../../sdk/definitions/itexture.h"
#include "../../sdk/definitions/imaterialsystem.h"
#include "../../sdk/interfaces/interfaces.h"
#include <unordered_map>
#include <vector>

namespace MaterialManager
{
	extern std::unordered_map<std::string, IMaterial*> m_Materials;
	extern std::unordered_map<std::string, ITexture*> m_Textures;

	void Init(void);
	void Unitialize(void);

	IMaterial* CreateMaterial(const std::string& name, const std::string& vmt);
	bool FreeMaterial(const std::string& name);
	bool MaterialExists(const std::string& name);
	IMaterial* GetMaterial(const std::string& name);

	ITexture* CreateTextureNamedRenderTarget(const std::string& name, int width, int height);
	bool FreeTexture(const std::string& name);
	bool TextureExists(const std::string& name);
	ITexture* GetTexture(const std::string& name);
};