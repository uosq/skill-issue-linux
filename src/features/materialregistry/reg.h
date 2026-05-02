#pragma once

#include <memory>
#include <vector>

#include "custom_material.h"

#define MATERIAL_DIR "./skill-issue/materials"

namespace MaterialRegistry
{
	void Initialize();
	void Shutdown();

	std::vector<std::shared_ptr<CustomMaterial>>& GetMaterials();
	std::shared_ptr<CustomMaterial> GetMaterialByName(const std::string& name);

	bool AddMaterial(const std::string& name, const std::string& vmt, std::shared_ptr<CustomMaterial>& out);
	bool RemoveMaterial(const std::string& name);

	bool SaveToDirectory(const std::string& dirPath);
	bool LoadFromDirectory(const std::string& dirPath);
} // end of namespace