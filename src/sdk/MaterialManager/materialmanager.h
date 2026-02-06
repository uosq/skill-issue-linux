#pragma once

#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/definitions/imaterial.h"
#include "../../sdk/definitions/itexture.h"
#include "../../sdk/definitions/imaterialsystem.h"
#include "../../sdk/interfaces/interfaces.h"
#include <unordered_map>
#include <vector>

class MaterialManager
{
private:
	std::unordered_map<std::string, IMaterial*> m_Materials;
	std::unordered_map<std::string, ITexture*> m_Textures;

public:
	MaterialManager()
	{
		m_Materials.reserve(10);
		m_Textures.reserve(10);
	}

	~MaterialManager()
	{
		for (auto mat : m_Materials)
			FreeMaterial(mat.first);
		
		for (auto tex : m_Textures)
			FreeTexture(tex.first);

		m_Materials.clear();
		m_Textures.clear();
	}

	IMaterial* CreateMaterial(const std::string& name, const std::string& vmt);
	bool FreeMaterial(const std::string& name);
	bool MaterialExists(const std::string& name);
	IMaterial* GetMaterial(const std::string& name);

	ITexture* CreateTextureNamedRenderTarget(const std::string& name, int width, int height);
	bool FreeTexture(const std::string& name);
	bool TextureExists(const std::string& name);
	ITexture* GetTexture(const std::string& name);
};

extern MaterialManager g_MaterialManager;