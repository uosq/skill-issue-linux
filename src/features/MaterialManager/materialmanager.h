#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "../../sdk/definitions/imaterial.h"
#include "../../sdk/definitions/imaterialsystem.h"
#include "../../sdk/definitions/itexture.h"
#include "../../sdk/definitions/keyvalues.h"
#include "../../sdk/interfaces/interfaces.h"

#include "../feature.h"

#include "custom_material.h"

#define MATERIAL_DIR "./skill-issue/materials"

class CMaterialManager
{
public:
	void Init();
	void Uninitialize();

	bool AddMaterial(const std::string& displayName, const std::string& vmt, std::shared_ptr<CustomMaterial>& out);
	bool RemoveMaterial(const std::string& displayName);
	std::shared_ptr<CustomMaterial> GetMaterialByName(const std::string& displayName);
	std::vector<std::shared_ptr<CustomMaterial>>& GetMaterials();
	std::optional<std::shared_ptr<CustomMaterial>> FindMaterial(uint32_t hash);

	void LoadMaterials();
	void LoadMaterialsFromDirectory(const std::string& dir_name);
	bool SaveToDirectory(const std::string& dirPath);

	ITexture* CreateTextureNamedRenderTarget(const std::string& name, int width, int height);
	bool FreeTexture(const std::string& name);
	bool TextureExists(const std::string& name);
	ITexture* GetTexture(const std::string& name);

	bool DoesMaterialExist(const std::string& displayName);

private:
	std::vector<std::shared_ptr<CustomMaterial>> m_Materials;
	std::unordered_map<std::string, ITexture*> m_Textures;
};

DECLARE_FEATURE(CMaterialManager, materials)