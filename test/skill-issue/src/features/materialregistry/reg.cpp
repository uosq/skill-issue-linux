#include "reg.h"
#include "custom_material.h"

#include <filesystem>
#include <fstream>

// checks if the material already exist in the 'materials' vector
static bool DoesMaterialExist(const std::vector<std::shared_ptr<CustomMaterial>>& materials, const std::string& name)
{
	for (auto it = materials.begin(); it != materials.end(); it++)
		if ((*it)->GetInternalName() == name)
			return true;

	return false;
}

bool MaterialRegistry::AddMaterial(const std::string& name, const std::string& vmt, std::shared_ptr<CustomMaterial>& out)
{
	if (DoesMaterialExist(materials, name))
		return false;

	materials.emplace_back(std::make_shared<CustomMaterial>(name, vmt));
	out = materials.back();

	return true;
}

bool MaterialRegistry::RemoveMaterial(const std::string& name)
{
	if (!DoesMaterialExist(materials, name))
		return true;

	for (auto it = materials.begin(); it != materials.end(); it++)
	{
		if ((*it)->GetInternalName() == name)
		{
			materials.erase(it);
			return true;
		}
	}

	return false;
}

std::shared_ptr<CustomMaterial> MaterialRegistry::GetMaterialByName(const std::string &name)
{
	if (!DoesMaterialExist(materials, name))
		return nullptr;

	for (auto it = materials.begin(); it != materials.end(); it++)
		if ((*it)->GetInternalName() == name)
			return (*it);

	return nullptr;
}

void MaterialRegistry::Init()
{
	std::shared_ptr<CustomMaterial> dummy_ptr;

	AddMaterial
	(
		"basic flat",

		"UnlitGeneric\n"
		"{\n"
		"\t$basetexture \"white\"\n"
		"}",

		dummy_ptr
	);

	AddMaterial
	(
		"basic shaded",

		"VertexLitGeneric\n"
		"{\n"
		"\t$basetexture \"white\"\n"
		"}",

		dummy_ptr
	);

	LoadFromDirectory(MATERIAL_DIR);
}

void MaterialRegistry::Shutdown()
{
	SaveToDirectory(MATERIAL_DIR);

	materials.clear();
}

std::vector<std::shared_ptr<CustomMaterial>>& MaterialRegistry::GetMaterials()
{
	return materials;
}

bool MaterialRegistry::SaveToDirectory(const std::string& dirPath)
{
	if (!std::filesystem::exists(dirPath))
		std::filesystem::create_directory(dirPath);

	for (const auto& mat : materials)
	{
		std::filesystem::path filePath = std::filesystem::path(dirPath) / (mat->GetDisplayName() + ".vmt");

		std::ofstream file(filePath);
		if (!file.is_open())
			continue;

		file << mat->GetVMT();
	}

	return true;
}

bool MaterialRegistry::LoadFromDirectory(const std::string& dirPath)
{
	if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
		return false;

	for (const auto& entry : std::filesystem::directory_iterator(dirPath))
	{
		if (entry.is_regular_file())
		{
			// get the file name without the extension
			std::string name = entry.path().stem().string();

			// read the entire file
			std::ifstream file(entry.path());
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string vmt = buffer.str();

			// check if the material already exists
			std::shared_ptr<CustomMaterial> mat = GetMaterialByName(name);

			if (!mat)
			{
				AddMaterial(name, vmt, mat);
				if (mat) mat->SetDisplayName(name);
			}
			else
			{
				// update existing material
				mat->SetVMT(vmt);
				mat->Refresh();
			}
		}
	}

	return true;
}